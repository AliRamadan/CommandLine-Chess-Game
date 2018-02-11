#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "ChessGame.h"
#include<windows.h>

int main()
{
    FILE *Save=fopen("Save&Load.txt","r");
    fclose(Save);
    FILE *FirstPlayer, *fp2, *fp3, *fp4 ;       //////////////////////files of player one
    FirstPlayer= fopen("FirstPlayerMoves.txt","w");
    fp3= fopen("fpRedo.txt","w");
    fclose(FirstPlayer);
    fclose(fp3);
    FILE *SecondPlayer, *sp2, *sp3, *sp4 ;      //////////////////////files of player two
    SecondPlayer = fopen("SecondPlayerMoves.txt","w");
    sp3= fopen("spRedo.txt","w");
    fclose(sp3);
    fclose(SecondPlayer);

    BuildBoard();///// Used only on time at the beginning
/*
**** this part is for loading The old if found a record and is totally Finished
*/
    Save=fopen("Save&Load.txt","r");
    if(!Save);
    else {
        int save;
        printf("For Loading press 1 for new game press 0 \n");
        scanf("%d",&save);
        while(save!=0 && save!=1 )
        {
            if(getchar()!='\n')
                fflush(stdin);
                scanf("%d",&save);
        }
        while(save==1)
        {
            if(getchar()!='\n'){
                fflush(stdin);
                scanf("%d",&save);
            }
            else
                break;
        }
        if(save==1){
            for(i=0;i<10;i++){
                for(j=0;j<14;j++){
                    fscanf(Save,"%c",&Board[i][j]);
                    printf("\t%c",Board[i][j]);
                }
            printf("\n\n");
            }
            fscanf(Save,"%d",&NoOfTurns);
        }
    }
    fclose(Save);
                        /**End Of Loading**/
while(CheckMate==0){
    fclose(fopen("fpRedo.txt", "w"));
    fclose(fopen("spRedo.txt", "w"));
    if(NoOfTurns%2==0){
    PrintBoard();
/**
*******     Find The King
**/
    int i,j,foundK=0;
    for(i=8;i>0;i--){
        for(j=1;j<9;j++){
            if(Board[i][j]=='K'){foundK=1;
            break;}
        }
        if(foundK==1)break;
    }
////////////////////////**************** checking stalemate & CheckMate ****************////////////////////////
    Check_FirstPlayer(i,j);            ///// now (i,j) are indicate the king place
    if(CheckingPieces>0 && CheckMate==1){
        checkmate_FirstPlayer(i,j);
        /*in this case the game is over*/
        if(CheckMate==1){
            printf("\a");
            MessageBox(0,"\t\tPlayer Two Won","CHESS GAME",1);
            break;}
    }
    Checking_StaleMate(i,j);
    if(StaleMate==1){
        printf("\a");
        MessageBox(0,"\t\tDRAW","CHESS GAME",1);
        break;}

/*
******
****** First Player Turn who plays with Upper cases pieces
******
*/
    printf("First Player Turn :\n");
    found =1;
    flag=1;
    do{
/*
********  Get the move with all validations
*/
        do{
            found=1;   flag=1;
            GetMove();
            DetectMove();
            while(found==0){
            printf("Invalid Move\n");
            GetMove();
            DetectMove();
            }
            CheckMove_FirstPlayer();
            CheckPieces_FirstPlayer();
            if(found==1||flag==1)
                printf("Invalid Move\n");
        }while(found==1||flag==1);
/**
***** because after all checking this move will be right then the new place of king is (n,m)
**/
        if(Board[x][y]=='K'){                   ////////in case player move the King
            Check_FirstPlayer(n,m);          ///// now (i,j) are indicate the king place
            if(CheckingPieces>0)printf("Check\a\n");
            if(CheckingPieces==0){
                    b=0;
                    if(islower(Board[n][m])){
                        if(a1<10){
                            b=12;
                            Board[a1][b]=Board[n][m];
                            ++a1;}
                        else{
                            a3=a1-10;
                            b=13;
                            Board[a3][b]=Board[n][m];
                            ++a1;}
                    }
                Board[n][m]=Board[x][y];
                if((x+y)%2==0)Board[x][y]='-';
                else Board[x][y]='.';
            }
        }
        else {                          /////////in case of any other move
                char temp1;          ///////////this is a temporary swapping
            temp1=Board[n][m];
            Board[n][m]=Board[x][y];
            if((x+y)%2==0)Board[x][y]='-';
            else Board[x][y]='.';
            Check_FirstPlayer(i,j);
            if(CheckingPieces>0)printf("Check\a\n");
            if(CheckingPieces==0){
                    b=0;
                    if(islower(temp1)){           /////saving move and killed pieces
                        if(a1<10){
                            b=12;
                            Board[a1][b]=temp1;
                            ++a1;}
                        else{
                            a3=a1-10;
                            b=13;
                            Board[a3][b]=temp1;
                            ++a1;}
                    }
            }
            else {                            /////////returning the old move and get another one
                Board[x][y]=Board[n][m];
                Board[n][m]=temp1;
                }
        }
    }while (CheckingPieces > 0);
    PrintBoard();
    NoOfTurns++;
///////////////////////////********* checking Promotion ***********////////////////////
    int p;
    for(p=1;p<9;p++){
        if (Board[1][p]=='P'){
        CheckPromotion_FirstPlayer(p);}
        }
/****************************** UNDO AND REDO ***************************************************/
    int Decide=1;
    u=0; v=0;
    noOfUndoFP=0; noOfUndoSP=0;
    while(Decide==1&&NoOfTurns!=0){
            printf("For undo Press [1] \nFor Redo Press [2] \nFor continue press [0] \n");
            scanf("%d",&Decide);
            while(v==0&&Decide==2)
            {
                printf("Invalid\n");
                scanf("%d",&Decide);
            }
            while(Decide==1)
            {
                if(getchar()!='\n'){
                    fflush(stdin);
                    scanf("%d",&Decide);
                }
                else
                    break;
            }
        while(Decide!=0 && Decide!=1 && Decide!=2){
            if(getchar()!='\n')
                fflush(stdin);
                scanf("%d",&Decide);}
        fflush(stdin);
        if(Decide==0||NoOfTurns==0) break;

        if(Decide==1){
            NoOfTurns=NoOfTurns-1;
            u=u+1;
            noOfUndoFP++;
            FirstPlayer=fopen("FirstPlayerMoves.txt","r");  //////////////reading form First player file
            for(i=0;i<noOfLinesFP;i++){
                fscanf(FirstPlayer,"%d %d %d %d %d %d",&x,&y,&n,&m,&A,&B);//// A and B are index of killed Pieces
                }
            /////////////////////////Promotion
        if(g>0&&n==1&&m==Promote1[g-1])
        {
            if(x==2 && (y==m+1 || y==m-1 || y==m)) {
            if(Board[1][Promote1[g-1]]=='Q'||Board[1][Promote1[g-1]]=='B'||Board[1][Promote1[g-1]]=='N'||Board[1][Promote1[g-1]]=='R'){
                    Board[1][Promote1[g-1]]='P';
                    g--;}
            }
        }

        Board[x][y]=Board[n][m];
        if (a1==0){
            if((n+m)%2==0)Board[n][m]='-';
            else Board[n][m]='.';
        }
        else {
            if(islower(Board[A][B])){
            Board[n][m]=Board[A][B];
            Board[A][B]='\0';
            a1=a1-1; }
            else {
                if((n+m)%2==0)Board[n][m]='-';
                else Board[n][m]='.';
                 }
        }

        rewind(FirstPlayer);
        fp2=fopen("copy.txt","w");
        fp3=fopen("fpRedo.txt","a");
        for(i=0;i<noOfLinesFP;i++){             //////////////reading form First player file
        fscanf(FirstPlayer,"%d %d %d %d %d %d",&x,&y,&n,&m,&A,&B);
        if(i != (noOfLinesFP-1)){
            fprintf(fp2,"%d %d %d %d %d %d",x,y,n,m,A,B);
            fprintf(fp2,"\n");
        }
        else {
            fprintf(fp3,"%d %d %d %d %d %d",x,y,n,m,A,B);
            fprintf(fp3,"\n");
            }
        }
        noOfLinesFP--;
        //close the files.
        fclose(FirstPlayer);
        fclose(fp2);
        fclose(fp3);
        remove("FirstPlayerMoves.txt");
        rename("copy.txt", "FirstPlayerMoves.txt"); }
//////////////////////////////////////////////////
        if(Decide==2&&v>0)
        {
            NoOfTurns=NoOfTurns+1;
            v=v-1;
            sp3 = fopen("spRedo.txt","r");
            for(i=0;i<noOfUndoSP;i++){
            fscanf(sp3,"%d %d %d %d %d %d",&x,&y,&n,&m,&A,&B);
            }
            if(isupper(Board[n][m])&&(A!=0||B!=0)){

                    Board[A][B]=Board[n][m];
                    Board[n][m]=Board[x][y];
                    a2=a2+1;
                    if((x+y)%2==0)Board[x][y]='-';
                    else Board[x][y]='.';
                }
            if (A==0&&B==0){
                        Board[n][m]=Board[x][y];
                        if((x+y)%2==0)Board[x][y]='-';
                        else Board[x][y]='.';
                }
                            //checking promotion
            int p;
            for(p=1;p<9;p++){
            if (Board[8][p]=='p')
                CheckPromotion_SecondPlayer(p);
        }
        rewind(sp3);
        sp4=fopen("copy4.txt","w");
        SecondPlayer=fopen("SecondPlayerMoves.txt","a");
        for(i=0;i<noOfUndoSP;i++){             //////////////reading form First player file
                fscanf(sp3,"%d %d %d %d %d %d",&x,&y,&n,&m,&A,&B);
                if(i != (noOfUndoSP-1)){
                fprintf(sp4,"%d %d %d %d %d %d",x,y,n,m,A,B);
                fprintf(sp4,"\n");
                }
                else {
                    fprintf(SecondPlayer,"%d %d %d %d %d %d",x,y,n,m,A,B);
                    fprintf(SecondPlayer,"\n");
                }
            }
        //close the files.
        fclose(SecondPlayer);
        fclose(sp4);
        fclose(sp3);
        remove("spRedo.txt");
        rename("copy4.txt", "spRedo.txt");
        noOfUndoSP--;
        noOfLinesSP++;
            Decide=1;
        }
        PrintBoard();
                ///////////////////////////////////////////////////////
        if(NoOfTurns>1){
        printf("For undo another move Press [1]\nFor Redo Press [2]\nFor continue press [0] \n");
        scanf("%d",&Decide);
        while(u==0&&Decide==2)
                {
                    printf("Invalid\n");
                    scanf("%d",&Decide);
                }
        while(Decide==1)
                {
                    if(getchar()!='\n'){
                        fflush(stdin);
                        scanf("%d",&Decide);
                    }
                    else
                        break;
                }
        while(Decide!=0 && Decide!=1 && Decide!=2){
                if(getchar()!='\n'){
                    fflush(stdin);
                    scanf("%d",&Decide);}
        else break;}

        if(Decide==0) break;
        if (Decide==1){
            NoOfTurns=NoOfTurns-1;
            v=v+1;
            noOfUndoSP++;
            SecondPlayer = fopen("SecondPlayerMoves.txt","r");
            for(i=0;i<noOfLinesSP;i++){
            fscanf(SecondPlayer,"%d %d %d %d %d %d",&x,&y,&n,&m,&A,&B);
                        }

            if(h>0&&n==8&&m==Promote2[h-1])
            {
                if(x==7 && (y==m+1 || y==m-1 || y==m) ) {
                if(Board[8][Promote2[h-1]]=='q'||Board[8][Promote2[h-1]]=='b'||Board[8][Promote2[h-1]]=='n'||Board[8][Promote2[h-1]]=='r'){
                        Board[8][Promote2[h-1]]='p';
                        h--;}
                }
            }
            Board[x][y]=Board[n][m];
            if (a2==0){
                    if((n+m)%2==0)Board[n][m]='-';
                    else Board[n][m]='.';
            }
            else {
                if(isupper(Board[A][B])){
                Board[n][m]=Board[A][B];
                Board[A][B]='\0';
                a2=a2-1; }
                else {
                    if((n+m)%2==0)Board[n][m]='-';
                    else Board[n][m]='.';
                     }

            }

            rewind(SecondPlayer);
            sp2=fopen("copy2.txt","w");
            sp3=fopen("spRedo.txt","a");
            for(i=0;i<noOfLinesSP;i++){             //////////////reading form First player file
                    fscanf(SecondPlayer,"%d %d %d %d %d %d",&x,&y,&n,&m,&A,&B);
                    if(i != (noOfLinesSP-1)){
                        fprintf(sp2,"%d %d %d %d %d %d",x,y,n,m,A,B);
                        fprintf(sp2,"\n");
                    }
                    else {
                        fprintf(sp3,"%d %d %d %d %d %d",x,y,n,m,A,B);
                        fprintf(sp3,"\n");
                    }
                }
                noOfLinesSP--;
            //close the files.
            fclose(SecondPlayer);
            fclose(sp2);
            fclose(sp3);
            remove("SecondPlayerMoves.txt");
            rename("copy2.txt", "SecondPlayerMoves.txt");
        }

        if(Decide==2&&u>0){
            NoOfTurns=NoOfTurns+1;
            u=u-1;
            fp3 = fopen("fpRedo.txt","r");
            for(i=0;i<noOfUndoFP;i++){
            fscanf(fp3,"%d %d %d %d %d %d",&x,&y,&n,&m,&A,&B);
            }

            if(islower(Board[n][m])&&(A!=0||B!=0)){
                    Board[A][B]=Board[n][m];
                    Board[n][m]=Board[x][y];
                    a1=a1+1;
                    if((x+y)%2==0)Board[x][y]='-';
                    else Board[x][y]='.';
                }
                if (A==0&&B==0){
                        Board[n][m]=Board[x][y];
                        if((x+y)%2==0)Board[x][y]='-';
                        else Board[x][y]='.';
                }
                        //checking promotion
            int p;
            for(p=1;p<9;p++){
            if (Board[1][p]=='P')
                CheckPromotion_FirstPlayer(p);
            }

            rewind(fp3);
            fp4=fopen("copy3.txt","w");
            FirstPlayer=fopen("FirstPlayerMoves.txt","a");
            for(i=0;i<noOfUndoFP;i++){             //////////////reading form First player file
                    fscanf(fp3,"%d %d %d %d %d %d",&x,&y,&n,&m,&A,&B);
                    if(i != (noOfUndoFP-1)){
                    fprintf(fp4,"%d %d %d %d %d %d",x,y,n,m,A,B);
                    fprintf(fp4,"\n");
                    }
                    else {
                        fprintf(FirstPlayer,"%d %d %d %d %d %d",x,y,n,m,A,B);
                        fprintf(FirstPlayer,"\n");
                    }
                }
            //close the files.
            fclose(FirstPlayer);
            fclose(fp4);
            fclose(fp3);
            remove("fpRedo.txt");
            rename("copy3.txt", "fpRedo.txt");
            noOfUndoFP--;
            noOfLinesFP++;

            Decide=1;
            }
        PrintBoard();
        }
    }
                            /**********Saving For Loading************************/
    Save=fopen("Save&Load.txt","w");
    for(i=0;i<10;i++){
        for(j=0;j<14;j++){
            fprintf(Save,"%c",Board[i][j]);
        }
    }
    fprintf(Save,"\n%d",NoOfTurns);
    fclose(Save);

    }
/****************************************************************************************************************************/
/*
******
****** Second Player Turn who plays with Lower cases pieces
******
*/
    else{
    PrintBoard();
/**
**     Find The King
**/
    int i,j,foundK=0;
    for(i=1;i<9;i++){
        for(j=1;j<9;j++){
            if(Board[i][j]=='k'){foundK=1;
            break;}
        }
        if(foundK==1)break;
    }
////////////////////////**************** checking stalemate & CheckMate ****************////////////////////////
    Check_SecondPlayer(i,j);            ///// now (i,j) are indicate the king place
    if(CheckingPieces>0 && CheckMate==1){
        checkmate_secondPlayer(i,j);
            /*in this case the game is over*/
        if(CheckMate==1){
            printf("\a");
            MessageBox(0,"\t\tPlayer One Won","CHESS GAME",1);
            break;}
    }
    Checking_StaleMate(i,j);
    if(StaleMate==1){
        printf("\a");
        MessageBox(0,"\t\tDRAW","CHESS GAME",1);
        break;}

    printf("Second Player Turn :\n");
    fflush(stdin);
    found=1;
    flag=1;

    do{
/*
********  Get the move with all validations
*/
    do{
        found=1;   flag=1;
        GetMove();
        DetectMove();
        while(found==0){
        printf("Invalid Move\n");
        GetMove();
        DetectMove();
        }
        CheckMove_SecondPlayer();
        CheckPieces_SecondPlayer();
        if(found==1||flag==1)
            printf("Invalid Move\n");
    }while(found==1 || flag==1 );
/**
**     because after all checking this move will be right then the new place of king is (n,m)
**/
        if(Board[x][y]=='k'){                   ////////in case player move the King
            Check_SecondPlayer(n,m);          ///// now (i,j) are indicate the king place
            if(CheckingPieces>0)printf("Check\a\n");
            if(CheckingPieces==0){
                b=0;
                if(isupper(Board[n][m])){                //////////////check whether second player will attack or move
                    if(a2<10){
                        b=10;
                        Board[a2][b]=Board[n][m];
                        ++a2;}
                    else{
                        b=11;
                        a4=a2-10;
                        Board[a4][b]=Board[n][m];
                        ++a2;}
                }
                Board[n][m]=Board[x][y];
                if((x+y)%2==0)Board[x][y]='-';
                else Board[x][y]='.';
            }
        }
        else {                          /////////in case of any other move
                char temp1;          ///////////this is a temporary swapping
            temp1=Board[n][m];
            Board[n][m]=Board[x][y];
            if((x+y)%2==0)Board[x][y]='-';
            else Board[x][y]='.';
            Check_SecondPlayer(i,j);
            if(CheckingPieces>0)
                printf("Check\a\n");
            if(CheckingPieces==0){
                b=0;
                if(isupper(temp1)){                //////////////check whether second player will attack or move
                    if(a2<10){
                        b=10;
                        Board[a2][b]=temp1;
                        ++a2;}
                    else{
                        b=11;
                        a4=a2-10;
                        Board[a4][b]=temp1;
                        ++a2;}
                }
            }
            else {                            /////////returning the old move and get another one
                Board[x][y]=Board[n][m];
                Board[n][m]=temp1;
            }
        }
    }while (CheckingPieces > 0);
    PrintBoard();
    NoOfTurns++;
///////////////////////////*********checking Promotion***********////////////////////
    int P;
    for(P=1;P<9;P++){
        if (Board[8][P]=='p'){
        CheckPromotion_SecondPlayer(P);}
        }
/***************************************** UNDO AND REDO  ********************************************************/
    int Decide=1;
    u=0; v=0;
    noOfUndoFP=0; noOfUndoSP=0;
    while(Decide==1&&NoOfTurns!=0){
            printf("For undo Press [1]\nFor Redo press [2]\nFor continue press [0] \n");
            scanf("%d",&Decide);
            while(u==0&&Decide==2)
            {
                printf("Invalid\n");
                scanf("%d",&Decide);
            }
            while(Decide==1)
            {
                if(getchar()!='\n'){
                    fflush(stdin);
                    scanf("%d",&Decide);
                }
                else
                    break;
            }
            while(Decide!=0 && Decide!=1 && Decide!=2)
            {
                    if(getchar()!='\n')
                    fflush(stdin);
                    scanf("%d",&Decide);
            }
        fflush(stdin);
        if(Decide==0 || NoOfTurns==0) break;

        if(Decide==1){
            NoOfTurns=NoOfTurns-1;
            v=v+1;
            noOfUndoSP++;
            SecondPlayer=fopen("SecondPlayerMoves.txt","r");//////////////reading form Second player file
            for(i=0;i<noOfLinesSP;i++){
                fscanf(SecondPlayer,"%d %d %d %d %d %d",&x,&y,&n,&m,&A,&B); //// A and B are index of killed Pieces
            }

            if(h>0&&n==8&&m==Promote2[h-1])
            {
                if(x==7&&(y==m+1 || y==m-1 || y==m)) {
                if(Board[8][Promote2[h-1]]=='q'||Board[8][Promote2[h-1]]=='b'||Board[8][Promote2[h-1]]=='n'||Board[8][Promote2[h-1]]=='r'){
                    Board[8][Promote2[h-1]]='p';
                    h--;}
                }
            }
            Board[x][y]=Board[n][m];
            if (a2==0){
                if((n+m)%2==0)Board[n][m]='-';
                else Board[n][m]='.';
            }
            else{
                if(isupper(Board[A][B])){
                Board[n][m]=Board[A][B];
                Board[A][B]='\0';
                a2=a2-1; }
                else {
                    if((n+m)%2==0)Board[n][m]='-';
                    else Board[n][m]='.';
                     }
            }

    rewind(SecondPlayer);
    sp2=fopen("copy2.txt","w");
    sp3=fopen("spRedo.txt","a");
    for(i=0;i<noOfLinesSP;i++){             //////////////reading form First player file
            fscanf(SecondPlayer,"%d %d %d %d %d %d",&x,&y,&n,&m,&A,&B);
            if(i != (noOfLinesSP-1)){
            fprintf(sp2,"%d %d %d %d %d %d",x,y,n,m,A,B);
            fprintf(sp2,"\n");
            }
            else {
                fprintf(sp3,"%d %d %d %d %d %d",x,y,n,m,A,B);
                fprintf(sp3,"\n");
            }
        }
    noOfLinesSP--;
    //close the files.
    fclose(SecondPlayer);
    fclose(sp2);
    fclose(sp3);
    remove("SecondPlayerMoves.txt");
    rename("copy2.txt", "SecondPlayerMoves.txt");
    }

    if(Decide==2&&u>0)
    {
        NoOfTurns=NoOfTurns+1;
        u=u-1;
        fp3 = fopen("fpRedo.txt","r");
        for(i=0;i<noOfUndoFP;i++){
        fscanf(fp3,"%d %d %d %d %d %d",&x,&y,&n,&m,&A,&B);
        }

        if(islower(Board[n][m])&&(A!=0||B!=0)){
                Board[A][B]=Board[n][m];
                Board[n][m]=Board[x][y];
                a1=a1+1;
                if((x+y)%2==0)Board[x][y]='-';
                else Board[x][y]='.';
            }
            if (A==0&&B==0){
                    Board[n][m]=Board[x][y];
                    if((x+y)%2==0)Board[x][y]='-';
                    else Board[x][y]='.';
            }
                        //checking promotion
        int p;
        for(p=1;p<9;p++){
        if (Board[1][p]=='P')
            CheckPromotion_FirstPlayer(p);
        }
        rewind(fp3);
        fp4=fopen("copy3.txt","w");
        FirstPlayer=fopen("FirstPlayerMoves.txt","a");
        for(i=0;i<noOfUndoFP;i++){             //////////////reading form First player file
            fscanf(fp3,"%d %d %d %d %d %d",&x,&y,&n,&m,&A,&B);
            if(i != (noOfUndoFP-1)){
            fprintf(fp4,"%d %d %d %d %d %d",x,y,n,m,A,B);
            fprintf(fp4,"\n");
            }
            else {
                fprintf(FirstPlayer,"%d %d %d %d %d %d",x,y,n,m,A,B);
                fprintf(FirstPlayer,"\n");
            }
        }
        //close the files.
        fclose(FirstPlayer);
        fclose(fp4);
        fclose(fp3);
        remove("fpRedo.txt");
        rename("copy3.txt", "fpRedo.txt");
        noOfUndoFP--;
        noOfLinesFP++;
            Decide=1;
        }
        PrintBoard();


            ///////////////////////////////////////////////////////
    if(NoOfTurns>=1){
    printf("For undo another move Press [1]\nFor Redo Press [2]\nFor continue press [0] \n");
    scanf("%d",&Decide);
    while(v==0&&Decide==2)
            {
                printf("Invalid\n");
                scanf("%d",&Decide);
            }
    while(Decide==1)
            {
                if(getchar()!='\n'){
                    fflush(stdin);
                    scanf("%d",&Decide);
                }
                else
                    break;
            }
    while(Decide!=0 && Decide!=1 && Decide!=2)
    {
        if(getchar()!='\n'){
        fflush(stdin);
        scanf("%d",&Decide);}
        else break;
    }
    if(Decide==0) break;
    if(Decide==1){
        NoOfTurns=NoOfTurns-1;
        u=u+1;
        noOfUndoFP++;
        FirstPlayer = fopen("FirstPlayerMoves.txt","r");
        for(i=0;i<noOfLinesFP;i++){
        fscanf(FirstPlayer,"%d %d %d %d %d %d",&x,&y,&n,&m,&A,&B);
        }

        if(g>0&&n==1&&m==Promote1[g-1])
            {
                if(x==2&&(y==m+1 || y==m-1 || y==m)) {
                if(Board[1][Promote1[g-1]]=='Q'||Board[1][Promote1[g-1]]=='B'||Board[1][Promote1[g-1]]=='N'||Board[1][Promote1[g-1]]=='R'){
                    Board[1][Promote1[g-1]]='P';
                    g--;}
                }
            }
            Board[x][y]=Board[n][m];
            if (a1==0){
                    if((n+m)%2==0)Board[n][m]='-';
                    else Board[n][m]='.';
            }
            else {
                if(islower(Board[A][B])){
                Board[n][m]=Board[A][B];
                Board[A][B]='\0';
                a1=a1-1;  }
                else {
                    if((n+m)%2==0)Board[n][m]='-';
                    else Board[n][m]='.';
                     }
            }

        rewind(FirstPlayer);
        fp2=fopen("copy.txt","w");
        fp3=fopen("fpRedo.txt","a");
        for(i=0;i<noOfLinesFP;i++){             //////////////reading form First player file
                fscanf(FirstPlayer,"%d %d %d %d %d %d",&x,&y,&n,&m,&A,&B);
                if(i != (noOfLinesFP-1)){
                fprintf(fp2,"%d %d %d %d %d %d",x,y,n,m,A,B);
                fprintf(fp2,"\n");
                }
                else {
                    fprintf(fp3,"%d %d %d %d %d %d",x,y,n,m,A,B);
                    fprintf(fp3,"\n");
                }
            }
            noOfLinesFP--;
        //close the files.
        fclose(FirstPlayer);
        fclose(fp2);
        fclose(fp3);
        remove("FirstPlayerMoves.txt");
        rename("copy.txt", "FirstPlayerMoves.txt");
        PrintBoard();
        }
    if(Decide==2&&v>0)
    {
        NoOfTurns=NoOfTurns+1;
        v=v-1;
        sp3 = fopen("spRedo.txt","r");
        for(i=0;i<noOfUndoSP;i++){
        fscanf(sp3,"%d %d %d %d %d %d",&x,&y,&n,&m,&A,&B);
        }
        if(isupper(Board[n][m])&&(A!=0||B!=0)){

                Board[A][B]=Board[n][m];
                Board[n][m]=Board[x][y];
                a2=a2+1;
                if((x+y)%2==0)Board[x][y]='-';
                else Board[x][y]='.';
            }
                if (A==0&&B==0){
                    Board[n][m]=Board[x][m];
                    if((x+y)%2==0)Board[x][y]='-';
                    else Board[x][y]='.';
            }
                    //Checking Promotion
    int p;
    for(p=1;p<9;p++){
        if (Board[8][p]=='p')
            CheckPromotion_SecondPlayer(p);
    }

    rewind(sp3);
    sp4=fopen("copy4.txt","w");
    SecondPlayer=fopen("SecondPlayerMoves.txt","a");
    for(i=0;i<noOfUndoSP;i++){             //////////////reading form First player file
            fscanf(sp3,"%d %d %d %d %d %d",&x,&y,&n,&m,&A,&B);
            if(i != (noOfUndoSP-1)){
            fprintf(sp4,"%d %d %d %d %d %d",x,y,n,m,A,B);
            fprintf(sp4,"\n");
            }
            else {
                fprintf(SecondPlayer,"%d %d %d %d %d %d",x,y,n,m,A,B);
                fprintf(SecondPlayer,"\n");
            }
        }
    //close the files.
    fclose(SecondPlayer);
    fclose(sp4);
    fclose(sp3);
    remove("spRedo.txt");
    rename("copy4.txt", "spRedo.txt");
    noOfUndoSP--;
    noOfLinesSP++;
        Decide=1;
    }
        PrintBoard();
        }
    }

                            /*Saving For Loading*/
    Save=fopen("Save&Load.txt","w");
    for(i=0;i<10;i++){
        for(j=0;j<14;j++){
            fprintf(Save,"%c",Board[i][j]);
        }
    }
    fprintf(Save,"\n%d",NoOfTurns);
    fclose(Save);
    }
}


    printf("\n\t\t\t===========================================\n\t\t\t===========================================\n");
    printf("\t\t\t*******************************************\n");
    printf("\t\t\t*******************************************\n");
    printf("\t\t\t*****************GAME OVER*****************\n");
    printf("\t\t\t*******************************************\n");
    printf("\t\t\t*******************************************\n");
    printf("\t\t\t===========================================\n\t\t\t===========================================\n");
    Save=fopen("Save&Load.txt","w");
    fclose(Save);
    return 0;
}
