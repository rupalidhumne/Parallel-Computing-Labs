{
    abc[0]=a;
    abc[1]=b; 
    abc[2]=c;
    return abc;
}
                            double T;
                            double abc[3]; //new array for each sphere
                            centerx=spheres[sphere][0];
                            centery=spheres[sphere][1]; 
                            centerz=spheres[sphere][2];
                            r=spheres[sphere][3];


                            
                            distancesBack[sphereBack]=TBack;



        if(minIndex==0)
                {
                   
                        startx=(rx*minVal)+ex; //actual distance of minT
                        starty=(ry*minVal)+ey; 
                        startz=(rz*minVal)+ez;


                }
        else
                {


                       
                        
                        normalMag = sqrt((startx*startx) + (starty*starty) + (startz*startz));
                        
                        normalX /= normalMag;
                        normalY /= normalMag;
                        normalZ /= normalMag;
                        

                }
		
              

                              double TBack; 
                               double abc[3]; //new array for each sphere
                            centerx=spheres[sphereBack][0];
                            centery=spheres[sphereBack][1]; 
                            centerz=spheres[sphereBack][2];
                            r=spheres[sphereBack][3];
                            


                            
                            distancesBack[sphereBack]=TBack;
                        }
                
 
                            
                    }
                }
                            {
                                  
                    

                             }

                
                            }
                          else
                          {
                              if(needShadow==1)
                              {
                                  rgb[y][x][0] = (int)(spheres[minIndex][4]*0.1);
                                  rgb[y][x][1] = (int)(spheres[minIndex][5]*0.1);
                                  rgb[y][x][2] = (int)(spheres[minIndex][6]*0.1);
                              }
                              else
                              {
                              if (dot>0.0)
                                    {
                                    rgb[y][x][0] = (int)(spheres[minIndex][4] *dot);
                                    rgb[y][x][1] = (int)(spheres[minIndex][5] *dot);
                                    rgb[y][x][2] = (int)(spheres[minIndex][6] *dot);
                                    }
                                    else
                                    {
                                        
                                            rgb[y][x][0] = (int)(spheres[minIndex][4] *-dot);
                                            rgb[y][x][1] = (int)(spheres[minIndex][5] *-dot);
                                            rgb[y][x][2] = (int)(spheres[minIndex][6] *-dot);
                                        
                                    }
                              }
                          }
                   
                }
            
          }
}


