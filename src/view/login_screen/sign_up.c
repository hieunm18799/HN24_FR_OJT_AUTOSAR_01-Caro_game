#include <stdio.h>
#include <conio.h>
#include "protocol.h"
#include <windows.h>
#include "top_screen.h"



void drawSignUpUI() {
    system("cls");
    gotoxy(60, 2);
    printf("[ Sign in ]");

    gotoxy(38, 8);
    printf("Caro Game");

    gotoxy(20, 10);
    printf("Username");

    gotoxy(30, 10);
    printf("[                    ]");

    gotoxy(20, 12);
    printf("Password");

    gotoxy(30, 12);
    printf("[                    ]");

    gotoxy(20, 14);
    printf("Re-enter password");

    gotoxy(30, 14);
    printf("[                    ]");

    gotoxy(35, 17);
    printf("[ Sign up ]");

    gotoxy(38, 19);
    printf("Back");

    currentScreen = VIEW_SIGN_UP; // Đặt màn hình hiện tại là màn hình đăng ký
}

void handleClickOnSignupScreen(){
    // Khi nhấn Sign In ở giao diện đăng ký
    if(MousePos.Y >= 10 && MousePos.Y <= 14 && MousePos.X >= 32 && MousePos.X <= 45){
        enterSignupCredentials(signup_username, signup_password, signup_reenterPassword); 
    }
    else if (MousePos.Y == 2 && MousePos.X >= 60 && MousePos.X <= 70) {
        drawSignInUI(); // Mở giao diện đăng nhập từ màn hình đăng ký
    }
    // Khi nhấn Back ở giao diện đăng ký
    else if (MousePos.Y == 19 && MousePos.X >= 35 && MousePos.X <= 40){
        drawInitialUI(); // Button Back mở giao diện ban đầu
    }
    // Khi nhấn Sign Up ở giao diện đăng ký
    else if (MousePos.Y == 17 && MousePos.X >= 30 && MousePos.X <= 40){
        signup(sockfd, signup_username, signup_password, signup_reenterPassword);
    }
}