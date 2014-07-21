//task x

flag[x] = true;
turn = !x;

//other task want it. not our turn
while(flag[!x] && turn !=x); 

//do context switch and task

flag[x] = false; // we finished, dont want it
