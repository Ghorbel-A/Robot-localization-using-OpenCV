/***********************************************************************************************************************
*																													  *
*								        											                                  *
*	        Traitement sur le Robot(detection de la position)			     			  *
*																													  *
*																													  *
***********************************************************************************************************************/

#include <windows.h>
#include <highgui.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "binarisation.h"
#include "pre_traitement.h"
#include "sobel.h"
#include "erode.h"
#include "cxtypes.h"


void robot_position()
{

		CvCapture *capture=NULL;
		IplImage *frame=NULL;
		char key='0';
		int X=0,Y=0,Ra=0,nb_frame=0;
		int X_robot=0,Y_robot=0;
		float X_robot_cm=0;
		float Y_robot_cm=0;
		FILE *fichier;
		FILE *fichier_courbe;
		char command[300];
		const char *gnupath;
		double temps;
        clock_t start;
		int ElementStructurant[5][5]={0,0,1,0,0, 0,1,1,1,0, 1,1,1,1,1, 0,1,1,1,0, 0,0,1,0,0
		};

        /******lecture video ******/
		
	   
		//capture = cvCaptureFromAVI("exemple2.avi");
	    capture = cvCreateCameraCapture(1);
		if (!capture) 
		{
 
		   printf("Ouverture du flux vidéo impossible !\n");
		   return 1;
		 
		}

        printf("\n");
		printf("*********************   Position  du  Robot    ***********************\n");printf("\n");
        printf("            Position en Pixels    |  Position en Cm   \n");printf("\n");

	    fichier=fopen("courbe.dat","w");
		fichier_courbe=fopen("courbe.gnu","w");

	
		if( capture )
		{
			while(key !='q' || key !='Q')
				
			    {
					/****** extraire les frames ******/

				if( cvGrabFrame( capture ))
					
					start = clock();
					frame = cvRetrieveFrame( capture,1 );
					if (frame == NULL)
					{ 
						
						break;
					}
					if(nb_frame==0)
						  {
                            
							/****** traitement sur l'image  ******/
							
							sobel(frame,0,frame->height,0,frame->width);
							ic_threshold(frame,63,0,frame->height,0,frame->width);
							Erosion(ElementStructurant,25,0,frame,0,frame->height,0,frame->width) ;

							//Erosion(ElementStructurant,25,0,frame);

							//Ouverture(ElementStructurant,25,0,frame);

							
							/****** detection du centre du Robot ******/
		 
							HoughCircleCompute(44,frame->height-(480-Y1),40,frame->width-167,27,27,frame,&X,&Y,&Ra);
							nb_frame++;
							
		                   
							/************* position en pixels ***********/

							X_robot=X-X1;    //X-X1;
							Y_robot=Y1-Y;    //Y1-Y;//(480-Y)-(480-Y1);
							

							


							/************** position en cm *************/

							X_robot_cm=((float)50/(X2-X1))*X_robot;
							Y_robot_cm=((float)50/((480-Y4)-(480-Y1)))*Y_robot;
							printf("                      X=%d       |      X cm =%.3f   \n",X_robot,X_robot_cm);
							printf("                      Y=%d               Y cm= %.3f   \n",Y_robot,Y_robot_cm);
							printf("\n");
							printf("\n");

							temps = (double)(clock()-start)/(double)CLOCKS_PER_SEC;
                            printf("\nRecherche terminée en %.2f seconde(s)!\n", temps);
						   // fprintf(fichier,"%.3f\n",temps);
														if (fichier != NULL)
								 {
									 fprintf(fichier,"%.3f %.3f\n",X_robot_cm,Y_robot_cm);
								 }

							if (!cvGrabFrame( capture ))
							break;
							if( !frame )
							break;
							cvShowImage( "Video", frame);
						  
							
					}
			  else if (nb_frame>=1)
				{
					/****** traitement sur l'image  ******/


					
					  
					sobel(frame,Y-Ra-10,Y+Ra+10,X-Ra-10,X+Ra+10);
					ic_threshold(frame,63,Y-Ra-10,Y+Ra+10,X-Ra-10,X+Ra+10);
					Erosion(ElementStructurant,25,0,frame,Y-Ra-10,Y+Ra+10,X-Ra-10,X+Ra+10) ;
				

					
				//	/****** detection du centre du Robot ******/
				
 
					HoughCircleCompute(Y-Ra-10,Y+Ra+10,X-Ra-10,X+Ra+10,27,27,frame,&X,&Y,&Ra);  

					/************* position en pixels ***********/

                    X_robot=X-X1;
					Y_robot=Y1-Y;//(480-Y)-(480-Y1);

					//if (fichier != NULL)
					//	 {  
					//		 fprintf(fichier,"%d %d\n",X_robot,Y_robot);
					//     }
					


					/************** position en cm *************/

					X_robot_cm=((float)50/(X2-X1))*X_robot;
					Y_robot_cm=((float)50/((480-Y4)-(480-Y1)))*Y_robot;
					printf("                      X=%d       |      X cm =%.3f   \n",X_robot,X_robot_cm);
					printf("                      Y=%d               Y cm= %.3f   \n",Y_robot,Y_robot_cm);
					printf("\n");
					printf("\n");


					temps = (double)(clock()-start)/(double)CLOCKS_PER_SEC;
				    printf("\nRecherche terminée en %.2f seconde(s)!\n", temps);
					
				  	if (fichier != NULL)
						 {  
							 fprintf(fichier,"%.3f %.3f\n",X_robot_cm,Y_robot_cm);
					     }
                    nb_frame++;

				    if (!cvGrabFrame( capture ))
					break;
					if( !frame )
					break;
					if(nb_frame>=60)break;
					cvShowImage( "Video", frame);
					key=cvWaitKey(1);
					
					}
			  }
	      }
                  fclose(fichier);
						if (fichier_courbe != NULL)
								  {
									fprintf(fichier_courbe,"unset key\n");
									fprintf(fichier_courbe,"set title \"Trajectoire du Robot\"\n");
									fprintf(fichier_courbe,"set xlabel \"x en pixels\" \n");
									fprintf(fichier_courbe,"set ylabel \"y en pixels\" \n");
									fprintf(fichier_courbe,"set grid\n");
									fprintf(fichier_courbe,"plot [0:100] [0:100] 'courbe.dat'lt 2\n");
									fprintf(fichier_courbe,"pause 100000 \n");
								  }
						fclose(fichier_courbe);
						system("start gnuplot\\wgnuplot.exe courbe.gnu");
}