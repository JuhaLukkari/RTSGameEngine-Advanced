#include "SDL.h"
#include "SDL_TTF.h"
#include <SDL_image.h>
#include <SDL_gfxPrimitives.h>

/*
C&C -clone by Juharts


TODO:
-Rahapaikat ja niiden toiminta
-Öljyrekan kulkeminen näihin paikkoihin
-Öljyrekan siirto omaan taloonsa ,nyt tankkitehtaassa

** Kaatuu samantien kun tein oljy systeemit eli sijotus ja liikuttelu
RIVILLÄ 475 ongelma

*/
SDL_Surface *naytto, *hiiri,*kuva[255][4],*tausta[15],*land[255],*talo[31],*panel,*icons[63]; // Pinnat
SDL_Surface *kohde,*bg,*icon[51][1];
SDL_Surface *menu,*panelIcons[63];
SDL_Surface *oljy[31];
SDL_Event tapahtuma;  //Tapahtumat 

int pois = 0; //mikä tilanne ohjelmassa
int hiiriX;
int hiiriY;
int napit;
int i=0;
int c=0;//rakennettavan yksikön numero
int bc=0;//rakennettavan rakennuksen numero
int ukot=3;//alku yksiköt
int ScrollSpeed=5;//Kartan vieritys nopeus
int gMenu=0;//onko menu päällä ? 0=ei 1=joo
int wtf=0;
int ValittuRakennus=0;//mikä rakennus tehdään
int rahat=1000;

void PiirraKuva(SDL_Surface *kuva, SDL_Surface *naytto, int x, int y);

bool unitconst=true;//voidaanko rakentaa yksiköitä

char puhver[100];

const int OilSiteAmmount=10;
const int unitSize=32;//yksiköiden koko pixeleinä
const int tankSize=141;//isompi koko pixeleinä(rakennukset)
const int palikoita=49;//jotain..
const int puut=30;//puiden määrä
const int IconSize=41;//kuvakkeiden koko pixeleinä
const int nurtsi=20000;//vihreitä pixeleit

struct soldier
{
	int x;
	int y;
	int hp;
	int frame;
	int orders;
	int dx;
	int dy;
	int owner;
	int TYPE;
	bool deploy;
}
;
struct building
{
	int x;
	int y;
	int hp;
	int orders;
	int size;
	int owner;
	int TYPE;
}
;
struct ground
{
	int x;
	int y;
}
;
struct accessory
{
	int x;
	int y;
}
;
struct ikoni
{
	int x;
	int y;
	int TYPE;
	int maxtime;
	int time;
}
;
struct ruoho
{
	int x;
	int y;
	
}
;
struct oljymesta
{
	int x;
	int y;
	int quant;
 
}
;

soldier Sotilaat[127];
building talot[100];
oljymesta oljypaikat[1];
ground grass[puut];
accessory accessories[palikoita];
ikoni ICON[51];
ruoho nurmikko[nurtsi];
/*
LiikuKohteeseen

Toiminta:Sotilaiden liikkumisen tekoäly
*/
void LiikuKohteeseen(int dx,int dy,int x,int y,int i)
{
   
	   if(x>dx)
			Sotilaat[i].x--;
		if(x<dx)
			Sotilaat[i].x++;
		if(y>dy)
			Sotilaat[i].y--;
		if(y<dy)
			Sotilaat[i].y++;
	   
	 
   

}
/*
ScrollMap

Toiminta:Kuva ruudun "liikkuminen" ,siirretään kaikkia osasia ScrollSpeedin verran suuntaan x
*/

void ScrollMap()
{
	if(hiiriX==0)
	{
		for(int i=0;i<bc;i++)
		{
			talot[i].x=talot[i].x+ScrollSpeed;
		}
		for(i=0;i<palikoita;i++)
		{
			accessories[i].x=accessories[i].x+ScrollSpeed;
		}

		for(int j=0;j<ukot-1;j++)
		{
			Sotilaat[j].x=Sotilaat[j].x+ScrollSpeed;
			Sotilaat[j].dx=Sotilaat[j].dx+ScrollSpeed;
		}
		
		for(int v=0;v<puut;v++)
		{
			grass[v].x=grass[v].x+ScrollSpeed;
		}
		for(int o=0;o<nurtsi;o++)
		{
			nurmikko[o].x=nurmikko[o].x+ScrollSpeed;
		}
	/*	for(int a=0;a<OilSiteAmmount;a++)
		{
           oljypaikat[a].x=oljypaikat[a].x+ScrollSpeed;
		}*/
		
	}
	if(hiiriX>1022)
	{
		
		for(int i=0;i<bc;i++)
		{
			talot[i].x=talot[i].x-ScrollSpeed;
		}
        for(i=0;i<49;i++)
		{
			accessories[i].x=accessories[i].x-ScrollSpeed;
		}
		for(int j=0;j<ukot-1;j++)
		{
			Sotilaat[j].x=Sotilaat[j].x-ScrollSpeed;
			Sotilaat[j].dx=Sotilaat[j].dx-ScrollSpeed;
		}
		for(int v=0;v<puut;v++)
		{
			grass[v].x=grass[v].x-ScrollSpeed;
		}
		for(int o=0;o<nurtsi;o++)
		{
			nurmikko[o].x=nurmikko[o].x-ScrollSpeed;
		}
	/*	for(int a=0;a<OilSiteAmmount;a++)
		{
           oljypaikat[a].x=oljypaikat[a].x-ScrollSpeed;
		}*/
		
	}
	if(hiiriY==0)
	{
		for(int i=0;i<bc;i++)
		{
			talot[i].y=talot[i].y+ScrollSpeed;
		}
        for(i=0;i<49;i++)
		{
			accessories[i].y=accessories[i].y+ScrollSpeed;
		}
		for(int j=0;j<ukot-1;j++)
		{
			Sotilaat[j].y=Sotilaat[j].y+ScrollSpeed;
			Sotilaat[j].dy=Sotilaat[j].dy+ScrollSpeed;
		}
		for(int v=0;v<puut;v++)
		{
			grass[v].y=grass[v].y+ScrollSpeed;
		}
		for(int o=0;o<nurtsi;o++)
		{
			nurmikko[o].y=nurmikko[o].y+ScrollSpeed;
		}
	/*	for(int a=0;a<OilSiteAmmount;a++)
		{
           oljypaikat[a].y=oljypaikat[a].y+ScrollSpeed;
		}*/
	}
	if(hiiriY>766)
	{
		for(int i=0;i<bc;i++)
		{
			talot[i].y=talot[i].y-ScrollSpeed;
		}

        for(i=0;i<49;i++)
		{
			accessories[i].y=accessories[i].y-ScrollSpeed;
		}

		for(int j=0;j<ukot-1;j++)
		{
			Sotilaat[j].y=Sotilaat[j].y-ScrollSpeed;
			Sotilaat[j].dy=Sotilaat[j].dy-ScrollSpeed;
		}
		for(int v=0;v<puut;v++)
		{
			grass[v].y=grass[v].y-ScrollSpeed;
		}
		for(int o=0;o<nurtsi;o++)
		{
			nurmikko[o].y=nurmikko[o].y-ScrollSpeed;
		}
	/*	for(int a=0;a<OilSiteAmmount;a++)
		{
           oljypaikat[a].y=oljypaikat[a].y-ScrollSpeed;
		}*/
	}

}

//Game_Menu
//
//Toiminta:Näyttää pelin päävalikon ja hävittää sen tarvittaessa ja pelin lopetus jos painaa exittiä
int Game_Menu(int gMenu)
{
    int MenuX=200;
	int MenuY=200;
    int BlockX=391;
	int BlockY=30;
    //Poistuminen
    if (napit && SDL_BUTTON(1) && hiiriX>MenuX && hiiriX<MenuX+IconSize && hiiriY>MenuY && hiiriY<MenuY+IconSize){
		gMenu=0;
		wtf=0;
	}
	//Main Menu,joskus tulevaisuudessa sellanen ehkä
	if (napit && SDL_BUTTON(1) && hiiriX>MenuX && hiiriX<MenuX+BlockX && hiiriY>MenuY+IconSize+BlockY && hiiriY<MenuY+IconSize+BlockY*2){
		pois=1;
		
	}
	//Exit
	if (napit && SDL_BUTTON(1) && hiiriX>MenuX && hiiriX<MenuX+BlockX && hiiriY>MenuY+IconSize+BlockY*10 && hiiriY<MenuY+IconSize+BlockY*13){
		pois=1;
		
	}
    if (gMenu==1)
    PiirraKuva(menu,naytto,200,200);
                                           
return 1;
}

 
 
//Rakennuksen toiminta 
//
//Toiminta:Joukkojen tuottaminen
void RakennuksenToiminta(int i)
{
	
	if (hiiriX>talot[i].x && hiiriX<talot[i].x+tankSize && hiiriY>talot[i].y && hiiriY<talot[i].y+tankSize)
	{
		if(talot[i].TYPE==1)
		{
			//piirretään ikonit
			ICON[0].x = talot[i].x;
			ICON[0].y = talot[i].y;		
			PiirraKuva(icons[0],naytto,ICON[0].x,ICON[0].y);
		}
		if(talot[i].TYPE==2)
		{
			//piirretään ikonit
			ICON[1].x = talot[i].x;
			ICON[1].y = talot[i].y;
			ICON[2].x = talot[i].x + IconSize + 5;
			ICON[2].y = talot[i].y;

			PiirraKuva(icons[1],naytto,ICON[1].x,ICON[1].y);
			PiirraKuva(icons[2],naytto,ICON[2].x,ICON[2].y);
		}

	}
//******************* ykkös kuvakkeiden käyttö taloissa *******************
if(napit && SDL_BUTTON(1) && hiiriX>talot[i].x && hiiriX<talot[i].x+IconSize && hiiriY>talot[i].y && hiiriY<talot[i].y+IconSize)
	   {
        if(unitconst==true)
		{
					talot[i].orders=1;				
					//jos rakennus on ukko talo ja rahaa on
					if(talot[i].TYPE==1 && rahat>=10){
					
								kuva[c][0]=SDL_LoadBMP("alfred.bmp");//laitetaan kuvat	
								kuva[c][1]=SDL_LoadBMP("alfredV.bmp");
								rahat = rahat - 10;                  // viedään rahat
								Sotilaat[c].TYPE=1;                  //solttu
								   Sotilaat[c].x=talot[i].x+70;      //alotuspaikka
									Sotilaat[c].y=talot[i].y+70;
									Sotilaat[c].orders=0;
									Sotilaat[c].dx=talot[i].x+70;
									Sotilaat[c].dy=talot[i].y+200;
									c++;
									ukot++;
									unitconst=false;						
							}
								
					//jos rakennus on vaunu talo ja rahaa on

					if(talot[i].TYPE==2 && rahat>=50){
						
								kuva[c][0]=SDL_LoadBMP("ifv.bmp");
								kuva[c][1]=SDL_LoadBMP("ifvv.bmp");
								rahat = rahat - 50;
								Sotilaat[c].TYPE=2;
								Sotilaat[c].x=talot[i].x+70;
									Sotilaat[c].y=talot[i].y+70;
									Sotilaat[c].orders=0;
									Sotilaat[c].dx=talot[i].x+70;
									Sotilaat[c].dy=talot[i].y+200;
									c++;
									ukot++;
									unitconst=false;															
							}					    
		}
	   } 
//***************** Kakkos kuvakkeiden käyttö taloissa *********************
if(napit && SDL_BUTTON(1) && hiiriX>talot[i].x+(IconSize+5) && hiiriX<talot[i].x+IconSize +(IconSize+5) && hiiriY>talot[i].y && hiiriY<talot[i].y+IconSize)
	{
		if(unitconst==true)
		{
			talot[i].orders=1;
			
			if(talot[i].TYPE==2 && rahat>=200)
			{
					kuva[c][0]=SDL_LoadBMP("ot.bmp");
					kuva[c][1]=SDL_LoadBMP("otv.bmp");
					rahat = rahat - 200;
					Sotilaat[c].TYPE=10;
					Sotilaat[c].x=talot[i].x+70;
                    Sotilaat[c].y=talot[i].y+70;
					Sotilaat[c].orders=0;
					Sotilaat[c].dx=talot[i].x+70;
					Sotilaat[c].dy=talot[i].y+200;
					c++;
					ukot++;
					unitconst=false;				
			}
		}

	}
}
//Tämä alue 
//piirto-funktioita  
//varten.
//
void PiirraKuva(SDL_Surface *kuva, SDL_Surface *naytto, int x, int y)
{
  SDL_Rect alue; // mille alueellä näyttöä kuva piirretään
  alue.x = x; // koordinaatit
  alue.y = y;
  SDL_BlitSurface(kuva, NULL, naytto, &alue); // piirto
}

void PiirraKuvaEX(SDL_Surface *kuva, SDL_Surface *naytto, int kuvax, int kuvay, int leveys, int korkeus, int nayttox, int nayttoy)
{
    SDL_Rect kuvaalue; // alue, mikä kuvasta piirretään
    kuvaalue.x = kuvax;
    kuvaalue.y = kuvay;
    kuvaalue.h = korkeus;
    kuvaalue.w = leveys;

    SDL_Rect nayttoalue; // alue näytöllä, jolle kuva piirretään
    nayttoalue.x = nayttox;
    nayttoalue.y = nayttoy;

    SDL_BlitSurface(kuva, &kuvaalue, naytto, &nayttoalue);
}

/////////
//int main(int argc,char* argv[])
//Toiminta:ohjelman pääohjelma
/////////
int main( int argc, char* argv[] )
{
	bool construction=true;
  

  SDL_Init(SDL_INIT_VIDEO);  
    naytto = SDL_SetVideoMode(1024, 768, 32, SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_FULLSCREEN);
    SDL_WM_SetCaption("Riskzation", NULL); 
    TTF_Init();
	 TTF_Font* font = TTF_OpenFont("armalite.ttf", 16);

   SDL_Color foregroundColor = { 0, 0, 255 };

//Tekstit
   SDL_Surface* textSurface = TTF_RenderText_Solid(font, "Game Menu",
      foregroundColor);

			
 SDL_Surface* tekstSurface = TTF_RenderText_Solid(font, puhver,
      foregroundColor);

 SDL_Rect textLocation = { 900, 10, 0, 0 };
 SDL_Rect tekstLocation= {10,10,0,0};
  
    //puut
	for(int l=0;l<puut;l++)
	{
		if(l%2==0)
			tausta[l]=IMG_Load("mud.jpg");
		else
     tausta[l]=IMG_Load("lolpuu.jpg");

     grass[l].x=rand()%1000-500;
	 grass[l].y=rand()%1000-500;
	}
	//kivet
    for(int s=0;s<50;s++)
	{
		land[s]=IMG_Load("kivi.jpg");
		accessories[s].x=s*75;
		accessories[s].y=rand()%1000-500;
       
	}
    //öljypaikat
/*	for(int b=0;b<2;b++)
	{
		oljypaikat[b].x=b;
		oljypaikat[b].y=b;
		oljypaikat[b].quant=1000;
	}*/

	 panel=SDL_LoadBMP("panel.bmp");				//paneeli
	 kohde=IMG_Load("slash.jpg");					//ruksi
	 menu=SDL_LoadBMP("menu.bmp");                  //menu
	 icons[0]=SDL_LoadBMP("riflesquad.bmp");        //mortti-ikoni
	 icons[1]=SDL_LoadBMP("ifvsquad.bmp");          //IFV-ikoni
	 icons[2]=SDL_LoadBMP("oiltruck.bmp");			//Öljyrekka-ikoni
	 panelIcons[0]=SDL_LoadBMP("vehfactoryIcon.bmp");//vaunutehtaan ikoni
	 panelIcons[1]=SDL_LoadBMP("barracksIcon.bmp");  //barracks-ikoni
	// bg=IMG_Load("ruoho2f.jpg");
 for(int f=1;f<ukot;f++)//alkusoltut
 {
	 kuva[0][0]=SDL_LoadBMP("ifv.bmp");
     kuva[0][1]=SDL_LoadBMP("ifvv.bmp");

	 kuva[f][0]=SDL_LoadBMP("alfred.bmp");
	
	 kuva[f][1]=SDL_LoadBMP("alfredV.bmp");

	Sotilaat[0].x=100;
	Sotilaat[0].y=100;
	Sotilaat[0].dx=100;
	Sotilaat[0].dy=100;
	Sotilaat[0].TYPE=1;
	Sotilaat[0].orders=0;
    Sotilaat[f].x=100+f*100;
    Sotilaat[f].y=100+f*100;
    Sotilaat[f].orders=0;
	Sotilaat[f].dx=100+f*100;
    Sotilaat[f].dy=100+f*100;
	c++;
 }
   talot[0].x=100;
   talot[0].y=100;
   talot[0].TYPE=99;
   talo[0]=SDL_LoadBMP("hq.bmp");//päämaja
   
   bc++;
 for(int o=0;o<nurtsi;o++) //nurmikon luominen
   {
	   nurmikko[o].x=rand()%4000-2000;
	   nurmikko[o].y=rand()%4000-2000;
   }

while(pois == 0) //PÄÄ-LOOPPI
{ 
	
    if(i==ukot-1)
		i=0;

	 SDL_PollEvent(&tapahtuma); 

			sprintf(puhver,"%i",rahat);//rahat bufferiin
            tekstSurface = TTF_RenderText_Solid(font, puhver,
      foregroundColor);


		napit=SDL_GetMouseState(&hiiriX, &hiiriY);
		 SDL_FillRect(naytto, NULL, SDL_MapRGB(naytto->format, 0, 100, 0));
	//	 PiirraKuva(bg,naytto,0,0);
	
	  if(napit & SDL_BUTTON(1) && Sotilaat[i].orders==1)//sotilaan liikuttelu
	   {
										
					Sotilaat[i].dx=hiiriX;
					Sotilaat[i].dy=hiiriY;
									
		PiirraKuva(kohde,naytto,hiiriX,hiiriY);
        
	   }
	  //sotilaan valinta
       if(napit & SDL_BUTTON(1) && hiiriX>Sotilaat[i].x && hiiriX<Sotilaat[i].x+unitSize && hiiriY>Sotilaat[i].y && hiiriY< Sotilaat[i].y+unitSize)
	   {        
		Sotilaat[i].orders=1;
		 construction=true;
		 unitconst=true;         
	   } 
	   if(napit & SDL_BUTTON(3) )//sotilas pois valinnasta
	   {
       
		Sotilaat[i].orders=0;
		      
	   } 
       //rakennettavan talon valinta
	   //ukko thedas   	   
        if(napit && SDL_BUTTON(1) && hiiriX>(1024-180) && hiiriX<(1024-180)+IconSize*2 && hiiriY>IconSize && hiiriY<IconSize*2)
		   ValittuRakennus=0;
		//vaunu tehdas
          if(napit && SDL_BUTTON(1) && hiiriX>(1024-180) && hiiriX<(1024-180)+IconSize && hiiriY>IconSize && hiiriY<IconSize*2)
		   ValittuRakennus=1;

	   if(napit & SDL_BUTTON(2))//talon rakennus
	   {
        if(construction==true)
		{
			
			//ukko tehdas
			if(ValittuRakennus==0 && rahat>=200){
				talo[bc]=SDL_LoadBMP("tehas.bmp");
				rahat= rahat - 200;
				talot[bc].TYPE=1;
				talot[bc].x=hiiriX;
				talot[bc].y=hiiriY;
				bc++;
				construction=false;
									}
			//vaunu tehdas
			if(ValittuRakennus==1 && rahat>=500){
				talo[bc]=SDL_LoadBMP("vehfactory.bmp");
				rahat= rahat - 500;
				talot[bc].TYPE=2;
				talot[bc].x=hiiriX;
				talot[bc].y=hiiriY;
				bc++;
				construction=false;
									}

			
		
		} 
	   } 
         for(int b=0;b<nurtsi;b++)//pisteitä maahan
		 {
			if(b>(nurtsi/2))
				pixelRGBA(naytto,nurmikko[b].x,nurmikko[b].y,0,190,0,255);
			else
                pixelRGBA(naytto,nurmikko[b].x,nurmikko[b].y,0,255,0,255);
		 }

	         for(int v=0;v<puut;v++)//puita
			 {
			  PiirraKuva(tausta[v],naytto,grass[v].x,grass[v].y); 
			 }
			  
			  for(int f=0;f<bc;f++)//talojen piirto
			  {
			   PiirraKuva(talo[f],naytto,talot[f].x,talot[f].y);
			   SDL_SetColorKey(talo[f], SDL_SRCCOLORKEY, SDL_MapRGB(talo[f]->format,0,0,0));
			   //if(talot[f].TYPE==0)
			   RakennuksenToiminta(f);
			  }
			   
			  for(int h=0;h<49;h++)//kivet
			  {
                PiirraKuva(land[h],naytto,accessories[h].x,accessories[h].y);
			  }

	   	 for(int a=0;a<ukot-1;a++)//ukkojen piirto ja liikuttelu niiden muistin mukaan
		 {
			 
			   LiikuKohteeseen(Sotilaat[a].dx,Sotilaat[a].dy,Sotilaat[a].x,Sotilaat[a].y,a);
			   for(int n=0;n<ukot-1;n++)
			   {
				   if(Sotilaat[a].x==Sotilaat[n].x && Sotilaat[a].y==Sotilaat[n].y && Sotilaat[a].dx==Sotilaat[n].x && Sotilaat[a].dy==Sotilaat[n].y && n!=a)
				   {
					   Sotilaat[a].x-=rand()%unitSize;
					   Sotilaat[a].y-=rand()%unitSize;
					   Sotilaat[a].dx=Sotilaat[a].x;
					   Sotilaat[a].dy=Sotilaat[a].y;

				   }
			   }   

			   if(Sotilaat[a].orders==1)//jos valittu
			   {
                 PiirraKuva(kuva[a][1],naytto,Sotilaat[a].x,Sotilaat[a].y);
			     SDL_SetColorKey(kuva[a][1], SDL_SRCCOLORKEY, SDL_MapRGB(kuva[a][1]->format,0,0,0));
			   }
			   if(Sotilaat[a].orders==0)//jos ei valittu
			   {
				 PiirraKuva(kuva[a][0],naytto,Sotilaat[a].x,Sotilaat[a].y);
			     SDL_SetColorKey(kuva[a][0], SDL_SRCCOLORKEY, SDL_MapRGB(kuva[a][0]->format,0,0,0));
			   }

			  
		 }
		 ScrollMap();//kartan scrollaus
		 //menuun pikanäppäimellä
		 if (tapahtuma.type ==SDL_KEYDOWN) {
			 if (tapahtuma.key.keysym.sym == SDLK_F10){
						gMenu=1;
					wtf=1;
			 }
				}

		 if (gMenu==1 && wtf==1)//jos f10 painettu..
			 Game_Menu(gMenu);//piirra menu

		 PiirraKuva(panel,naytto,1024-180,0);//paneelin piirto
		 PiirraKuva(panelIcons[0],naytto,1024-170,IconSize);//tehdas
		 PiirraKuva(panelIcons[1],naytto,1024-170+IconSize+10,IconSize);

		 SDL_BlitSurface(textSurface, NULL, naytto, &textLocation);//menu teksti paneelin päälle
		 SDL_BlitSurface(tekstSurface,NULL,naytto, &tekstLocation);//rahojen näyttö
				SDL_Flip(naytto);//kaikki näytölle
                
				
                

				if ( tapahtuma.type == SDL_QUIT )  {  pois = 1;  }
				if ( tapahtuma.type == SDL_KEYDOWN )  {
					if ( tapahtuma.key.keysym.sym == SDLK_ESCAPE ) { pois = 1; }
				}
			 
  i++;
 
 }
 SDL_Quit();
 TTF_Quit();
  return 0;
}