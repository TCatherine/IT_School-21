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
		auto my_downloader = std::make_unique<Downloader>();
		my_downloader->get_file("https://protei.ru/themes/custom/aga/favicon.ico", "picture_2.png");
}

int main(int, char**) {
    test_1();
    test_2(); 
}
