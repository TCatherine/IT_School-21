/*
    Here are small tests for validating class methods
*/
#include <iostream>
#include "downloader.h"

void test_1(){
    /* this test is for constructor validation */
    Downloader my_downloader("https://protei.ru/themes/custom/aga/favicon.ico", "picture_1.png");
}

void test_2(){
    /* this test is to test the file save method */
    Downloader* my_downloader = new Downloader();
    my_downloader->get_file("https://protei.ru/themes/custom/aga/favicon.ico", "picture_2.png");
    delete my_downloader;
}

int main(int, char**) {
    test_1();
    test_2(); 
}
