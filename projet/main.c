#include <windows.h>
#include <windows.h>
#include <highgui.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "binarisation.h"
#include "Hough cercles.h"
#include "sobel.h"
#include "erode.h"
#include "pre_traitement.h"
#include "cxtypes.h"
int main(int argc, char** argv) 

{
    /* variables locales */

	CvCapture *capture=NULL;
	IplImage *frame=NULL;
	int X=0,Y=0,Ra=0,nb_frame=0;
	int X_robot=0,Y_robot=0;
	float X_robot_cm=0;
	float Y_robot_cm=0;
	char key='0';
    char buffer[256];
	char reponse[256];
	float old_position[2];
	float new_position[2];
	float diff_position[2];
	int tab[8];
	int iTabSize;
    int i=0,nId, nBytesWritten, nBytesRead;
	int xf=400,yf=400,xe,ye;
	int d;
	int xo=0,yo=0;
	float x1,y1,l;
	float Theta = 0.785398163, dTheta;/*orientation initiale du robot*/
    int L = 300; /*distance entre les roues en mm*/
    double WHEEL_RADIUS = 7.81;
    int AXLE_LENGTH = 53;
    double  MM_PER_ENC = 3.14159 * 2 * WHEEL_RADIUS / 600;
    FILE *fichier;
	FILE *fichier_courbe;
	double temps;
    clock_t start;
    int ElementStructurant[5][5]={0,0,1,0,0, 0,1,1,1,0, 1,1,1,1,1, 0,1,1,1,0, 0,0,1,0,0
		};


	fichier=fopen("courbe.dat","w");
	fichier_courbe=fopen("courbe.gnu","w");
    printf("%d\n",CLOCKS_PER_SEC);

		    /* demande du numéro du port COM */
    printf("Entrez le numero du port COM : ");
    scanf("%d", &nId);
	/* tentative d'ouverture */
    printf("Ouverture et configuration du port COM%d...\r\n", nId);
    if(!OpenCOM(nId)) return -1;
    printf("...OK\r\n");
    pre_traitement();
    capture = cvCreateCameraCapture(1);

	if (!capture) 
		{
 
		   printf("Ouverture du flux vidéo impossible !\n");
		   return 1;
		 
		}
	printf("\n");
	printf("*********************   Position  du  Robot    ***********************\n");printf("\n");
    printf("            Position en Pixels    |  Position en Cm   \n");printf("\n");
	/*xe= xf - xo;
	ye= yf - yo;
    d =(int) (sqrt ( xe*xe + ye*ye)); 	*/
  
	/* reset encoders */
	WriteCOM("G,0,0\r", strlen("G,0,0\r"), &nBytesWritten); 
	ReadCOM(buffer, sizeof(buffer)-1, &nBytesRead);
    
	/* calcule old position */
    WriteCOM("H\r", strlen("H\r"), &nBytesWritten);
	ReadCOM(buffer, sizeof(buffer)-1, &nBytesRead);
	buffer[nBytesRead] = '\0';
	printf("%d octet(s) recu(s) :\n%s\n", nBytesRead, buffer);
    iTabSize =  sizeof(tab)/sizeof(tab[0]);	   		        	
	parseBuffer(tab,&iTabSize,buffer);
		for(i=0;i<2;i++)
		{
		old_position[i]= (tab[i] * MM_PER_ENC);
		printf("%f\t\n",old_position[i]);
		}
   //WriteCOM("D,5,5\r", strlen("D,5,5\r"), &nBytesWritten);
   //ReadCOM(buffer, sizeof(buffer)-1, &nBytesRead);
 //while (d>10)
 // {
	if(capture)
	{
	while(key !='q' || key !='Q')
	   {					
				if( cvGrabFrame( capture ))
				  {	
					start = clock();
					frame = cvRetrieveFrame( capture,1 );

					if(nb_frame==0)
				    {

						sobel(frame,0,frame->height,0,frame->width);
						ic_threshold(frame,60,0,frame->height,0,frame->width);
						//Erosion(ElementStructurant,25,0,frame,0,frame->height,0,frame->width) ;
						HoughCircleCompute(50,frame->height-(480-Y1),100,frame->width-100,27,29,frame,&X,&Y,&Ra);
						nb_frame++;
						X_robot=X-X1;    //X-X1;
						Y_robot=Y1-Y;    //Y1-Y;//(480-Y)-(480-Y1);

							/************** position en cm *************/
				    	X_robot_cm=((float)50/(X2-X1))*X_robot;
						Y_robot_cm=((float)50/((480-Y4)-(480-Y1)))*Y_robot;
						printf("                      X=%d       |      X cm =%.3f   \n",X_robot,X_robot_cm*10);
						printf("                      Y=%d               Y cm= %.3f   \n",Y_robot,Y_robot_cm*0);
						printf("\n");
						printf("\n");

						temps = (clock_t)(clock()-start)/CLOCKS_PER_SEC;
                        printf("\nRecherche terminée en %.3f seconde(s)!\n", temps);

						if (fichier != NULL)
								 {
									 fprintf(fichier,"%.3f %.3f\n",X_robot_cm*10,Y_robot_cm*10);
								 }
						xo=X_robot_cm*10;
						yo=Y_robot_cm*10;
				    	xe= xf - X_robot_cm*10;
	                    ye= yf - Y_robot_cm*10;
                        d =(int)(sqrt ( xe*xe + ye*ye)); 


						cvShowImage( "Video", frame);
						WriteCOM("D,3,3\r", strlen("D,3,3\r"), &nBytesWritten);
                        ReadCOM(buffer, sizeof(buffer)-1, &nBytesRead);
						if (!cvGrabFrame( capture ))
						break;
						if( !frame )
						break;
						
						//key=cvWaitKey(1);

					}

					else if (nb_frame>=1 && d>20)
					{
		               /* WriteCOM("D,3,3\r", strlen("D,3,3\r"), &nBytesWritten);
                        ReadCOM(buffer, sizeof(buffer)-1, &nBytesRead);*/
						sobel(frame,Y-Ra-7,Y+Ra+7,X-Ra-7,X+Ra+7);
					    ic_threshold(frame,60,Y-Ra-7,Y+Ra+7,X-Ra-7,X+Ra+7);
					    Erosion(ElementStructurant,25,0,frame,Y-Ra-7,Y+Ra+7,X-Ra-7,X+Ra+7) ;
						HoughCircleCompute(Y-Ra-7,Y+Ra+7,X-Ra-7,X+Ra+7,27,29,frame,&X,&Y,&Ra); 
						printf("%d %d \t\n" ,X,480-Y);

						X_robot=X-X1;
					    Y_robot=Y1-Y;//(480-Y)-(480-Y1);

						X_robot_cm=((float)50/(X2-X1))*X_robot;
						Y_robot_cm=((float)50/((480-Y4)-(480-Y1)))*Y_robot;
						printf("                      X=%d       |      X cm =%.3f   \n",X_robot,X_robot_cm * 10);
						printf("                      Y=%d               Y cm= %.3f   \n",Y_robot,Y_robot_cm * 10);
						printf("\n");
						printf("\n");
                        
						temps = (clock_t)(clock()-start)/CLOCKS_PER_SEC;
                        printf("\nRecherche terminée en %.4f seconde(s)!\n", temps);
					
				  		if (fichier != NULL)
							 {  
								 fprintf(fichier,"%.3f %.3f %.4f\n",X_robot_cm * 10,Y_robot_cm *10,temps);
							 }	

						if (!cvGrabFrame( capture ))
						break;
						if( !frame )
						break;
						cvShowImage( "Video", frame);
                        
						key=cvWaitKey(1);
						nb_frame++;
						printf("d=%d\n",d);
						WriteCOM("H\r", strlen("H\r"), &nBytesWritten);		
						ReadCOM(buffer, sizeof(buffer)-1, &nBytesRead);
						buffer[nBytesRead] = '\0';
						parseBuffer(tab,&iTabSize,buffer);

						for(i=0;i<2;i++)
							{
							new_position[i]= (tab[i] * MM_PER_ENC);
							diff_position[i]= new_position[i] - old_position[i];
							}

						/*for(i=0;i<2;i++)
							{
								diff_position[i]= new_position[i] - old_position[i];

						}*/
						dTheta = ((diff_position[0] * (-1) + diff_position[1] * (1)) / AXLE_LENGTH);
						l = ((diff_position[0] * (1)) + (diff_position[1] * (1))) / 2;
						x1 = xo + cos(Theta + dTheta/2)*l;
						y1 = yo + sin(Theta + dTheta/2)*l;
						Theta = Theta + dTheta;

						if (fichier != NULL)
							 {
								 fprintf(fichier,"%f %f\n",x1,y1);
							 }

						/*distance entre le robot et le point objectif*/
						for(i=0;i<2;i++)
							 {
								 old_position[i]= new_position[i];
							
							 }

						xe = xf - x1;
						ye = yf - y1;
						d =(int) (sqrt(xe*xe+ye*ye));
						xo = x1;
						yo = y1;
						if(d<=20)
						{
							WriteCOM("D,0,0\r", strlen("D,0,0\r"), &nBytesWritten);
							cvShowImage( "Video", frame);
							sobel(frame,Y-Ra-7,Y+Ra+7,X-Ra-7,X+Ra+7);
							ic_threshold(frame,60,Y-Ra-7,Y+Ra+7,X-Ra-7,X+Ra+7);
							Erosion(ElementStructurant,25,0,frame,Y-Ra-7,Y+Ra+7,X-Ra-7,X+Ra+7) ;
							HoughCircleCompute(Y-Ra-7,Y+Ra+7,X-Ra-7,X+Ra+7,27,29,frame,&X,&Y,&Ra); 
							printf("%d %d \t\n" ,X,480-Y);

							X_robot=X-X1;
							Y_robot=Y1-Y;//(480-Y)-(480-Y1);

							X_robot_cm=((float)50/(X2-X1))*X_robot;
							Y_robot_cm=((float)50/((480-Y4)-(480-Y1)))*Y_robot;
							printf("                      X=%d       |      X cm =%.3f   \n",X_robot,X_robot_cm * 10);
							printf("                      Y=%d               Y cm= %.3f   \n",Y_robot,Y_robot_cm * 10);
							printf("\n");
							printf("\n");
					        
							temps = (clock_t)(clock()-start)/CLOCKS_PER_SEC;
							printf("\nRecherche terminée en %.4f seconde(s)!\n", temps);
						
							if (fichier != NULL)
								 {  
									 fprintf(fichier,"%.3f %.3f %.4f\n",X_robot_cm * 10,Y_robot_cm *10,temps);
								 }				
							//cvShowImage( "Video", frame);
							
						/*	cvReleaseCapture(&capture);
							cvDestroyWindow("video");*/
							fclose(fichier);
								
								if (fichier_courbe != NULL)
								{
									
									fprintf(fichier_courbe,"unset key\n");
									fprintf(fichier_courbe,"set title \'Trajectoire du Robot\'\n");
									fprintf(fichier_courbe,"set xlabel \'x en pixels\' \n");
									fprintf(fichier_courbe,"set ylabel \'y en pixels\' \n");
									fprintf(fichier_courbe,"set grid\n");
									fprintf(fichier_courbe,"plot [0:640] [0:480] \"courbe.dat\" \n");
									fprintf(fichier_courbe,"pause 100000 \n");
									
								}
								fclose(fichier_courbe);
								system("start gnuplot\\wgnuplot.exe courbe.gnu");
							}
						}
				}
	
			}
					
}

    CloseCOM();
	
	system("pause");
	return 0;
     
}

                           