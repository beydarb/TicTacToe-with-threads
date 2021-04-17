#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdbool.h>
#include <time.h>

int turn = 0;
bool stop = false;
struct arg_struct{
	int size;
	char **matrix;
	int winner;
};

bool horizontalWin(int size, char **board, int * winner){
	int r,c;
	for (r=0; r<size; r++){
		for (c = 1; c < size; c++) {
            if ((board[r][c] != 'X' && board[r][c] !='O') || board[r][c] != board[r][c-1]) {
                break; 
            }
        }
        if(c == size){
	    	if(board[r][0] == 'X')
	    		*winner = 1;
	    	else
	    		*winner = 0;
	    	return true;
	    }
    }
    return false;
}

bool verticalWin(int size, char **board, int * winner){

	int r,c;
	for(c = 0; c < size; c++){
		for(r = 1; r < size; r++){
			if((board[r][c] != 'X' && board[r][c] !='O') || board[r][c] != board[r-1][c]){
				break;
			}
		}
		if(r == size){
			
			if(board[0][c] == 'X')
				*winner = 1;
			else
				*winner = 0;
			return true;
		}
	}
	return false;
}

bool diagonalWin(int size, char **board, int * winner){
	int r,c;
	//first diagonal
	for(r= 1; r < size; r++){
		c = r;
		if((board[r][c] != 'X' && board[r][c] !='O') || board[r][c] != board[r-1][c-1]){
			break;
		}
	}
	if(r == size){
		if(board[r-1][r-1] == 'X'){
			*winner=1;
		}
		else
			*winner = 0;
		return true;
	}
	c = 0;
	for(r = size-1; r > 0; r--){
		if((board[r][c] != 'X' && board[r][c] !='O') || board[r][c] != board[r-1][c+1]){
			break;
		}
		c++;
	}
	if(r == 0){
		if(board[r][size-1] == 'X'){
			*winner=1;
		}
		else
			*winner = 0;
		return true;
	}
	return false;
}
bool checkIsFull(int size, char **table){
	int count = 0;
	for(int i = 0; i < size; i++){
		for(int j = 0; j < size; j ++){
			if(table[i][j] == 'X' || table[i][j] == 'O'){
				count++;
			}
		}
	}
	
	if(count== size*size)
		return true;
	else
		return false;

}

void *playerX(void* arguments){
	while(true){
		int temp;
		while(turn != 0);
		if(stop){
			pthread_exit(NULL);
		}
		struct arg_struct *args = arguments;
		int n = args->size;
		int x = rand() % n;
		int y = rand() % n;
		while(checkIsFull(n, args->matrix) == false && (args->matrix[x][y] == 'X' || args->matrix[x][y] == 'O')){
			x = rand() % n;
			y = rand() % n;
		}
		if(!checkIsFull(n, args->matrix)){
			args->matrix[x][y] = 'X';
			printf("Player X played on: (%d , %d)\n", x, y);
		}
		if(horizontalWin(n, args->matrix, &temp)){
			stop = true;
			turn = 1;
			pthread_exit(NULL);
		}
		else if(verticalWin(n, args->matrix, &temp)){
			stop = true;
			turn = 1;
			pthread_exit(NULL);
		}
		else if(diagonalWin(n, args->matrix, &temp)){
			stop = true;
			turn = 1;
			pthread_exit(NULL);
		}
		turn = 1;
		if(checkIsFull(n, args->matrix) == true){
			pthread_exit(NULL);
	
		}
	}
}

void *playerO(void* arguments){
	while(true){
		int temp;
		while(turn !=1);
		if(stop){
			pthread_exit(NULL);
		}
		struct arg_struct *args = arguments;
		int n = args->size;
		int x = rand() % n;
		int y = rand() % n;
		while(checkIsFull(n, args->matrix) == false && (args->matrix[x][y] == 'X' || args->matrix[x][y] == 'O')){
			x = rand() % n;
			y = rand() % n;
		}

		if(!checkIsFull(n, args->matrix)){
			args->matrix[x][y] = 'O';
			printf("Player O played on: (%d , %d)\n", x, y);
		}
		if(horizontalWin(n, args->matrix, &temp)){
			stop = true;
			turn = 0;
			pthread_exit(NULL);
		}
		else if(verticalWin(n, args->matrix, &temp)){
			stop = true;
			turn = 0;
			pthread_exit(NULL);
		}
		else if(diagonalWin(n, args->matrix, &temp)){
			stop = true;
			turn = 0;
			pthread_exit(NULL);
		}
		turn = 0;
		if(checkIsFull(n, args->matrix) == true){
			pthread_exit(NULL);
		}
	}
}

int main(int argc, char *argv[])
{
	srand(time(NULL));
	int temp_winner;
	int winner = 5;

	printf("Board Size: %s\n" , argv[1]);
	int size = atoi(argv[1]);

	struct arg_struct *v = (struct arg_struct *) malloc(sizeof(struct arg_struct));
	v->size = size;
	v->winner = temp_winner;
	v->matrix = (char **) malloc(size* sizeof(char *));
	for (int z = 0; z < size; ++z)
	{
    	v->matrix[z] = (char *) malloc(size * sizeof(char));
 
	}
 
	pthread_t thread1, thread2;

	pthread_create(&thread1, NULL, playerX, (void*) v);
	pthread_create(&thread2, NULL, playerO, (void*) v);

	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);

	printf("%s\n", "Game end");
	char win;
	if(horizontalWin(size, v->matrix, &winner)){
		if(winner == 1)
			win = 'X';
		else
			win = 'O';
		printf("Winner is: %c\n", win);

	}
	else if(verticalWin(size, v->matrix, &winner)){
		if(winner == 1)
			win = 'X';
		else
			win = 'O';
		printf("Winner is: %c\n", win);
	}

	else if(diagonalWin(size, v->matrix, &winner)){
		if(winner == 1)
			win = 'X';
		else
			win = 'O';
		printf("Winner is: %c\n", win);
	}
	else if(checkIsFull(size, v->matrix)){
		printf("It is a tie\n");
	}
	for(int i= 0; i < size; i++){
		for(int j = 0; j < size; j++){
			if(v->matrix[i][j] != 'X' && v->matrix[i][j] != 'O')
				printf("[ ]");
			else
				printf("[%c]", v->matrix[i][j]);
		}
		printf("\n");
	}

	return 0;
}