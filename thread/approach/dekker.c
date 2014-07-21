bool flag = {false, false};
int turn = 0;
// task x
flag[x] = true;
while(flag[!x])
{
	if(turn != x)
	{
		flag[x] = false;
		while(turn != x);
		flag[x] = true;
	}
}

// Do Work - CS

turn = !x;
flag[x] = false;

