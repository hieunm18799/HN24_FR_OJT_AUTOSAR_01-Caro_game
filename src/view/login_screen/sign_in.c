#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include "top_screen.h"


void drawSignInUI() {
    system("cls");
    gotoxy(60, 2);
    printf("[ Sign up ]");

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

    gotoxy(35, 15);
    printf("[ Sign in ]");

    gotoxy(38, 17);
    printf("Back");

    currentScreen = 1; // Đặt màn hình hiện tại là màn hình đăng nhập
}

void handleClickOnSigninScreen(){
    //Khi nhấn Sign Up ở giao diện đăng nhập
    if (MousePos.Y == 2 && MousePos.X >= 60 && MousePos.X <= 70) {
        drawSignUpUI(); // Mở giao diện đăng ký từ màn hình đăng nhập
    }
    // Khi nhấn Back ở giao diện đăng nhập
    else if (MousePos.Y == 17 && MousePos.X >= 35 && MousePos.X <= 40){
        drawInitialUI(); // Button Back mở giao diện ban đầu
    }
    // Khi nhấn Sign In ở giao diện đăng nhập
    else if (MousePos.Y == 15 && MousePos.X >= 30 && MousePos.X <= 40){
        // Hàm check đăng nhập ở đây

    }
}