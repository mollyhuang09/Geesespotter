#include "geesespotter_lib.h"
#include "geesespotter.h"

/*
EARLY SUBMIT
allocate a char array with xdim * ydim elements and initialize each with value 0 
*/
char *createBoard (std::size_t xdim, std::size_t ydim){
    char *board {new char[xdim * ydim]{}};

    for (int k = 0; k < (xdim*ydim); k++){
        board[k] = 0;
    }

    return board;
}

/*
EARLY SUBMIT
deallocate the given board
*/
void cleanBoard (char *board){
    delete[] board;
    board = NULL;
}

/*
EARLY SUBMIT
print the content of the board to "std::cout"
*/
void printBoard (char *board, std::size_t xdim, std::size_t ydim){
   
    for (int k = 0; k < ydim; k++){
        for (int j = 0; j < xdim; j++){
            
            // if field is marked 
            if ((board[(k*xdim)+j] & markedBit()) == markedBit() ){
                std::cout << "M";
            // if field is hidden
            }else if ((board[(k*xdim)+j] & hiddenBit()) == hiddenBit() ){
                std::cout << "*";
            //print the field value 
            }else { 
                std::cout << (board[(k*xdim)+j] & valueMask());
            }
        }
        
        std::cout << "\n"; 
    }
}

//FINAL SUBMIT
void computeNeighbors (char *board, std::size_t xdim, std::size_t ydim){ 

    for (int k = 0; k < ydim; k++){
        for (int j = 0; j < xdim; j++) {
            int numOfGeese = 0;
            // if there is no goose in the field 
            if ((board [k*xdim+j] & valueMask()) != 9){

                if ( (j+1) < xdim) { 
                    // field to the right 
                    if ( (((board[((k*xdim)+j)+1]) & valueMask()) == 9) ){
                        numOfGeese ++; 
                    }
                    // field above and to the right 
                    if ( (((board[((k*xdim)+j)-xdim+1]) & valueMask()) == 9) ){
                        numOfGeese ++;
                    }
                    //field below and to the right 
                    if ( (((board[((k*xdim)+j)+xdim+1]) & valueMask()) == 9) ){
                        numOfGeese ++;
                    }
                }

                if ( j > 0 ) {
                    //field to the left 
                    if ( (((board[((k*xdim)+j)-1]) & valueMask()) == 9) ){
                        numOfGeese ++;
                    }
                    //field above and to the left 
                    if ( (((board[((k*xdim)+j)-xdim-1]) & valueMask()) == 9) ){
                        numOfGeese ++;
                    }
                    //field below and to the left 
                    if ( (((board[((k*xdim)+j)+xdim-1]) & valueMask()) == 9) ){
                        numOfGeese ++;
                    }
                }

                //if field above has goose 
                if (( (k*xdim+j-xdim) >= 0 ) && (((board[((k*xdim)+j)-xdim]) & valueMask()) == 9) ){
                    numOfGeese ++; 
                }

                //if field below has goose 
                if ( ((k*xdim+j+xdim) < xdim*ydim) && (((board[((k*xdim)+j)+xdim]) & valueMask()) == 9) ){
                    numOfGeese ++;
                }

                board [k*xdim+j] =  (board [k*xdim+j] & valueMask());
                board [k*xdim+j] |= numOfGeese;
            }
        }
    }
}



/*
EARLY SUBMIT
hide all the field values 
*/
void hideBoard (char *board, std::size_t xdim, std::size_t ydim){
    for (int k = 0; k < xdim * ydim; k++){
        board[k] |= hiddenBit();
    }
}

//FINAL SUBMIT
int reveal (char *board, std::size_t xdim, std::size_t ydim, std::size_t xloc, std::size_t yloc){

    // if field is marked 
    if ((board [yloc*xdim + xloc] & markedBit()) == markedBit()){
        return 1;
    }
    // if field is revealed (not hidden) 
    else if ((board [yloc*xdim + xloc] & hiddenBit()) != hiddenBit()){
        return 2;
    }
    // if field has goose 
    else if ((board [yloc*xdim + xloc] & valueMask()) == 9){
        board [yloc*xdim + xloc] |= 9; //&=  (~hiddenBit()); 
        return 9; 
    } 

    else {
        // if field is 0 
        if ((board [yloc*xdim + xloc] & valueMask()) == 0){
            board [yloc*xdim + xloc] &= (~hiddenBit());
        
            // checks if the field is on the most right 
            if ( (xloc+1) < xdim) { 
                board [yloc*xdim + xloc+1] &= (~hiddenBit());
                board [yloc*xdim + xloc-xdim+1] &= (~hiddenBit());
                board [yloc*xdim + xloc+xdim+1] &= (~hiddenBit());
            }

            // checks if field is on the most left 
            if ( xloc > 0 ) {
                board [yloc*xdim + xloc-1] &= (~hiddenBit());
                board [yloc*xdim + xloc-xdim-1] &= (~hiddenBit());
                board [yloc*xdim + xloc+xdim-1] &= (~hiddenBit());
            }

            // checks if field is on the first row 
            if ((yloc*xdim+xloc-xdim) > 0){
                board [yloc*xdim + xloc-xdim] &= (~hiddenBit());
            }

            // checks if field is on the last row 
            if ((yloc*xdim+xloc+xdim) < xdim*ydim){
                board [yloc*xdim + xloc+xdim] &= (~hiddenBit());
            }
        }
        else {
            board [yloc*xdim+xloc] &= (~hiddenBit());
        }
    }

    return 0; 
}

/*
EARLY SUBMIT
if the field is already revealed, states doesn't change and returns 2 
otherwise, function returns zero 
*/
int mark (char *board, std::size_t xdim, std::size_t ydim, std::size_t xloc, std::size_t yloc) {
    
    // if field is revealed  
    if (board [(yloc*xdim) + xloc] & hiddenBit() != hiddenBit() ) {
        return 2; 
    
    }
    
    // if the field is hidden 
    else {
        board [(yloc*xdim) + xloc] ^= markedBit();
        return 0;
    }
}


//FINAL SUBMIT
bool isGameWon (char *board, std::size_t xdim, std::size_t ydim) {
    
    for (int k = 0; k < (xdim*ydim); k++){
        // there is no goose in the field 
        if ((board[k] & valueMask()) != 9){
            // if field is hidden
            if ((board[k] & hiddenBit()) == hiddenBit()) {
                return false; 
            }
        }
    }

    return true; 
}