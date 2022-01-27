void loadHighScore(){
  PACKFILE *highscore;
  highscore = pack_fopen("data\\hscore.dat", "rp");
  
  h_score = pack_igetl(highscore);
  
  pack_fclose(highscore);
}

void resetHighScore(){
  h_score = 0;
}

void saveHighScore(){
  PACKFILE *highscore;
  highscore = pack_fopen("data\\hscore.dat", "wp");
               
  if(score>h_score) h_score = score;
  
  pack_iputl(h_score,highscore);
  pack_fclose(highscore);
}
