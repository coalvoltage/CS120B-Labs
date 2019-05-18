//
//  main.c
//  ReverseArray
//
//  Created by james luo on 5/14/19.
//  Copyright © 2019 james luo. All rights reserved.
//

#include <stdio.h>
unsigned char myAry [16];
void LoadAry (){
    char *myWord = "Message";
    unsigned char curAry = 15;
    unsigned char targetLimit = 3;
    for (int i = targetLimit; i >= 0; i -= 1 ){
        printf("%c", myWord[i]);
        myAry[curAry] = myWord[i];
        curAry -= 1;
    }
    while (curAry < 16){
        myAry[curAry] = 0;
        curAry -= 1;
    }
}

int main(int argc, const char * argv[]) {
    // insert code here...
    LoadAry();
//printf("%c", &myAry[15]);
    return 0;
}


