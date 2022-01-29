//------------------------------------------------------------------------------
//
// LOAD MAP
//
//------------------------------------------------------------------------------

void loadMap(int number){
     
  FILE* mapfile;
  char mapname[64];
  sprintf(mapname, "data\\levels\\level%d.txt", number);
  mapfile = fopen(mapname, "r");
  
  int i,j;
  for(j=0; j < MAP_HEIGHT; j++){
    for(i=0; i < MAP_WIDTH; i++){
    
      int c = fgetc(mapfile) - 48;
      
      while(!(c>=0 && c<=9)) c = fgetc(mapfile) - 48;
      
      map.layer[BASE_L].cell[j][i].id = c;
    
    }
  }
  
  fclose(mapfile);
  
}

//------------------------------------------------------------------------------
//
// DRAW MAP
//
//------------------------------------------------------------------------------

void drawMap(){
     
  int i,j;
  int first_tile = TILE1 - 1;
  
  for(j=0; j < MAP_HEIGHT; j++){
    for(i=0; i < MAP_WIDTH; i++){
      
      blit( (BITMAP*)data[first_tile+map.layer[BASE_L].cell[j][i].id].dat,
            DoubleBuffer, 0, 0,
            i * TILE_SIZE, j * TILE_SIZE,
            TILE_SIZE, TILE_SIZE
            );
    
    }
  }
  
  int first_item = ITEM01 - 1;
  
  for(j=0; j < MAP_HEIGHT; j++){
    for(i=0; i < MAP_WIDTH; i++){
      
      if(map.layer[OBJECT_L].cell[j][i].id > 0){
                                           
        draw_sprite(DoubleBuffer,
                    (BITMAP*)data[first_item+map.layer[OBJECT_L].cell[j][i].id].dat,
                    i * TILE_SIZE, j * TILE_SIZE
                    );
              
      }
    
    }
  }
     
}
