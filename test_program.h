#ifndef TEST_PROGRAM_H
#define TEST_PROGRAM_H


#include<iostream>
#include"line_formation_control.h"
#include<opencv2/core.hpp>
#include"imageprocess.h"
#include<opencv2/opencv.hpp>
#include"formation_control.h"
#include"utils.h"
#include"udp_client.h"
using namespace std;

void test_pnp();
void test_formation_alg();
void test_udp();

void test_split_string();
void test_line_formation();
void test_parse_str();
void test_potential_field_two_points();

void test_rep_potential_field_line_formation();
#endif // TEST_PROGRAM_H
