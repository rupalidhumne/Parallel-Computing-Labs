else
            {
              
                currentX += 0.001*normalX;
                currentY += 0.001*normalY;
                currentZ += 0.001*normalZ;
            }
                
                //If the light intersects any sphere then the point is in shadow
                int w = 0;

                 
                    else
                    {
                        if(lightIntersect==true)
                        {
                        }
                    }
                }
                else
                {
                     double shadeDotProduct = dot(normalX, rlx, normalY, rly, normalZ, rlz);

                    if(lightIntersect==true)
                    {
                        rgb[y][x][0] = (int)(spheres[minSphere][4]*0.1);
                        rgb[y][x][1] = (int)(spheres[minSphere][5]*0.1);
                        rgb[y][x][2] = (int)(spheres[minSphere][6]*0.1);
                    }
                    else
                    {   
                        if (shadeDotProduct>0)
                        {
                        rgb[y][x][0] = (int)(spheres[minSphere][4] *shadeDotProduct);
                        rgb[y][x][1] = (int)(spheres[minSphere][5] *shadeDotProduct);
                        rgb[y][x][2] = (int)(spheres[minSphere][6] *shadeDotProduct);
                        }
                        else
                        {
                        rgb[y][x][0] = (int)(spheres[minSphere][4] *-shadeDotProduct);
                        rgb[y][x][1] = (int)(spheres[minSphere][5] *-shadeDotProduct);
                        rgb[y][x][2] = (int)(spheres[minSphere][6] *-shadeDotProduct);
                        }
        
