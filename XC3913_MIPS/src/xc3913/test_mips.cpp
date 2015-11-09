//
//  main.cpp
//  MIPS_simulator
//
//  Created by Xiaohan Chen on 20/10/2014.
//  Copyright (c) 2014 Xiaohan Chen. All rights reserved.
//

#include "mips.h"
#include<iostream>
using namespace std;

int main()
{
    
//    uint32_t invert = 0x01020304;
//    
//    invert_bytes(&invert);
//    
//    cout<<invert<<"hahah"<<endl;                  ***test for invert_byte function
    
    
    
    
    mips_mem_h mem=mips_mem_create_ram(
                                       1<<20,
                                       4                //32bits, 4bytes
                                       );
    
    mips_cpu_h cpu=mips_cpu_create(mem);                //create CPU, pc = 0;

    uint32_t branched_addr = 0;   //PC value after branch;
    uint32_t current_addr = 0;
    uint32_t return_addr = 0;
    uint32_t test_addr = 0;
    uint32_t mflo = 0, mfhi = 0;
    uint32_t data_mem = 0, data_32 = 0, data_32_post = 0;

    /////////////////////////////////
    //testing AND
    
    mips_test_begin_suite();

    int testId=mips_test_begin_test("and");             //get the ID
    int passed=0;
    
    mips_error err = mips_cpu_set_register(cpu, 8, 0x0000FFFFul);               //unsigned long
    if(err==0)
        err = mips_cpu_set_register(cpu, 9, 0x00FFFF00ul);
    
    uint32_t AND_instr = 0x1095024;    // R_type, 8 , 9 , 10, 00000, 100100
    invert_bytes(&AND_instr);
  
    if(err==0)
        err = mips_cpu_get_pc(cpu, &current_addr);
    
    if(err==0)
        err = mips_mem_write(mem,current_addr,4,(uint8_t*)&AND_instr);
    
    if(err==0){
        cout<<"CPU running and"<<endl;
        err=mips_cpu_step(cpu);
    }
    
    uint32_t got = 0, reg_lo = 0, reg_hi = 0;
    if(err==0)
        err = (mips_error)(err | mips_cpu_get_register(cpu, 10, &got));
    
    passed = (err == mips_Success) && (got== 0x0000FF00);
    mips_test_end_test(testId, passed, NULL);
    
    
    ///////////////////////////////////////////////////////////////////
    //testing OR function
    
    testId=mips_test_begin_test("or");             //get the ID
    passed=0;
    
    err = mips_cpu_set_register(cpu, 8, 0x0000FFFFul);               //unsigned long
    if(err==0)
        err = mips_cpu_set_register(cpu, 9, 0x00FFFF00ul);
    
    AND_instr = 0x1095025;    // R_type, 8 , 9 , 10, 00000, 100101
    invert_bytes(&AND_instr);
    if(err==0)
        err = mips_cpu_get_pc(cpu, &current_addr);
    
    if(err==0)
        err = mips_mem_write(mem,current_addr,4,(uint8_t*)&AND_instr);
    
    if(err==0){
        cout<<"CPU running or"<<endl;
        err=mips_cpu_step(cpu);
    }
    
    if(err==0)
        err = (mips_error)(err | mips_cpu_get_register(cpu, 10, &got));
    
    passed = (err == mips_Success) && (got== 0x00FFFFFF);
    mips_test_end_test(testId, passed, NULL);
    
    //////////////////////////////
    //testing XOR
    
    
    testId=mips_test_begin_test("xor");             //get the ID
    passed=0;
    
    err = mips_cpu_set_register(cpu, 8, 0x0000FFFFul);               //unsigned long
    if(err==0)
        err = mips_cpu_set_register(cpu, 9, 0x00FFFF00ul);
    
    AND_instr = 0x1095026;    // R_type, 8 , 9 , 10, 00000, 100110
    invert_bytes(&AND_instr);
    if(err==0)
        err = mips_cpu_get_pc(cpu, &current_addr);
    
    if(err==0)
        err = mips_mem_write(mem,current_addr,4,(uint8_t*)&AND_instr);
    
    if(err==0){
        cout<<"CPU running xor"<<endl;
        err=mips_cpu_step(cpu);
    }
    
    if(err==0)
        err = (mips_error)(err | mips_cpu_get_register(cpu, 10, &got));
    
    passed = (err == mips_Success) && (got== 0x00FF00FF);
    mips_test_end_test(testId, passed, NULL);
    ///////////////////////////////////////////////
    //testing SLTU
    
    testId=mips_test_begin_test("sltu");             //get the ID
    passed=0;
    
    err = mips_cpu_set_register(cpu, 8, 0xFFFFFFFFul);               //unsigned long
    if(err==0)
        err = mips_cpu_set_register(cpu, 9, 0x00000001ul);
    
    AND_instr = 0x109502B;    // R_type, 8 , 9 , 10, 00000, 100110
    invert_bytes(&AND_instr);
    if(err==0)
        err = mips_cpu_get_pc(cpu, &current_addr);
    
    if(err==0)
        err = mips_mem_write(mem,current_addr,4,(uint8_t*)&AND_instr);
    
    if(err==0){
        cout<<"CPU running sltu"<<endl;
        err=mips_cpu_step(cpu);
    }
    
    if(err==0)
        err = (mips_error)(err | mips_cpu_get_register(cpu, 10, &got));
    
    passed = (err == mips_Success) && (got== 0x00000000);
    mips_test_end_test(testId, passed, NULL);
    
    /////////////////////////////////////////////////////////
    //testing SLT
    
    testId=mips_test_begin_test("slt");             //get the ID
    passed=0;
    
    err = mips_cpu_set_register(cpu, 8, 0xFFFFFFFFul);               //unsigned long
    if(err==0)
        err = mips_cpu_set_register(cpu, 9, 0x00000001ul);
    
    AND_instr = 0x109502A;    // R_type, 8 , 9 , 10, 00000, 100110
    invert_bytes(&AND_instr);
    if(err==0)
        err = mips_cpu_get_pc(cpu, &current_addr);
    
    if(err==0)
        err = mips_mem_write(mem,current_addr,4,(uint8_t*)&AND_instr);
    
    if(err==0){
        cout<<"CPU running slt"<<endl;
        err=mips_cpu_step(cpu);
    }
    
    if(err==0)
        err = (mips_error)(err | mips_cpu_get_register(cpu, 10, &got));
    
    passed = (err == mips_Success) && (got== 0x1);
    mips_test_end_test(testId, passed, NULL);
    
    
    
    
    /////////////////////////////////////////////////////////
    //testing ADDU without overflow
    
    testId=mips_test_begin_test("addu");             //get the ID
    passed=0;
    
    err = mips_cpu_set_register(cpu, 8, 0xFFFFFFFFul);               //unsigned long
    if(err==0)
        err = mips_cpu_set_register(cpu, 9, 0x00000010ul);
    
    AND_instr = 0x1095021;    // R_type, 8 , 9 , 10, 00000, 100110
    invert_bytes(&AND_instr);
    if(err==0)
        err = mips_cpu_get_pc(cpu, &current_addr);
    
    if(err==0)
        err = mips_mem_write(mem,current_addr,4,(uint8_t*)&AND_instr);
    
    if(err==0){
        cout<<"CPU running addu"<<endl;
        err=mips_cpu_step(cpu);
    }
    
    if(err==0)
        err = (mips_error)(err | mips_cpu_get_register(cpu, 10, &got));
    
    passed = (err == mips_Success) && (got== 0x0000000F);
    mips_test_end_test(testId, passed, NULL);
    
    //////////////////////////////
    //testing SUBU without overflow, unsigned!!!!!!!!!!!!!!!
    
    testId=mips_test_begin_test("subu");             //get the ID
    passed=0;
    
    err = mips_cpu_set_register(cpu, 8, 0x00000007ul);               //unsigned long
    if(err==0)
        err = mips_cpu_set_register(cpu, 9, 0x00000008ul);
    
    AND_instr = 0x1095023;    // R_type, 8 , 9 , 10, 00000, 100110
    invert_bytes(&AND_instr);
    if(err==0)
        err = mips_cpu_get_pc(cpu, &current_addr);
    
    if(err==0)
        err = mips_mem_write(mem,current_addr,4,(uint8_t*)&AND_instr);
    
    if(err==0){
        cout<<"CPU running subu"<<endl;
        err=mips_cpu_step(cpu);
    }
    
    if(err==0)
        err = (mips_error)(err | mips_cpu_get_register(cpu, 10, &got));
    
    passed = (err == mips_Success) && (got== 0xFFFFFFFF);
    mips_test_end_test(testId, passed, NULL);
    
    /////////////////////////////////////////
    // testing ADD;

    testId=mips_test_begin_test("add");             //get the ID
    passed=0;
    
    err = mips_cpu_set_register(cpu, 8, 0xFFFFFFFEul);               //unsigned long
    if(err==0)
        err = mips_cpu_set_register(cpu, 9, 0x00000001ul);
    
    AND_instr = 0x1095020;    // R_type, 8 , 9 , 10, 00000, 100110
    invert_bytes(&AND_instr);
    if(err==0)
        err = mips_cpu_get_pc(cpu, &current_addr);
    
    if(err==0)
        err = mips_mem_write(mem,current_addr,4,(uint8_t*)&AND_instr);
    
    if(err==0){
        cout<<"CPU running add"<<endl;
        err=mips_cpu_step(cpu);
    }
    if(err==0)           //if overflows occurs, test stops but desk register is still writen.
        err = (mips_error)(err | mips_cpu_get_register(cpu, 10, &got));         //stored in -14, Oxfffffff2,
    passed = (err == mips_Success) && (got== 0xFFFFFFFF);
    mips_test_end_test(testId, passed, NULL);
    
    ///////////////////////////////////////////
    //testing SUB;
    
    testId=mips_test_begin_test("sub");             //get the ID
    passed=0;
    
    err = mips_cpu_set_register(cpu, 8, 0x00000002ul);               //unsigned long
    if(err==0)
        err = mips_cpu_set_register(cpu, 9, 0x00000003ul);
    
    AND_instr = 0x1095022;    // R_type, 8 , 9 , 10, 00000, 100110
    invert_bytes(&AND_instr);
    if(err==0)
        err = mips_cpu_get_pc(cpu, &current_addr);
    
    if(err==0)
        err = mips_mem_write(mem,current_addr,4,(uint8_t*)&AND_instr);
    
    if(err==0){
        cout<<"CPU running sub"<<endl;
        err=mips_cpu_step(cpu);
    }
    if(err==0)           //if overflows occurs, test stops but desk register is still writen.
        err = (mips_error)(err | mips_cpu_get_register(cpu, 10, &got));         //stored in -14, Oxfffffff2,
    passed = (err == mips_Success) && (got== 0xFFFFFFFF);
    mips_test_end_test(testId, passed, NULL);
    
    ///////////////////////////////////////////
    //testing SRA
    
    testId=mips_test_begin_test("sra");             //get the ID
    passed=0;
    
    err = mips_cpu_set_register(cpu, 8, 0x00000000ul);               //unsigned long
    if(err==0)
        err = mips_cpu_set_register(cpu, 9, 0xF0000F00ul);
    
    AND_instr = 0x1095103;    // R_type, 8 , 9 , 10, 00000, 100110
    invert_bytes(&AND_instr);
    if(err==0)
        err = mips_cpu_get_pc(cpu, &current_addr);
    
    if(err==0)
        err = mips_mem_write(mem,current_addr,4,(uint8_t*)&AND_instr);
    
    if(err==0){
        cout<<"CPU running sra"<<endl;
        err=mips_cpu_step(cpu);
    }
    if(err==0)           //if overflows occurs, test stops but desk register is still writen.
        err = (mips_error)(err | mips_cpu_get_register(cpu, 10, &got));         //stored in -14, Oxfffffff2,
    passed = (err == mips_Success) && (got== 0xFF0000F0);
    mips_test_end_test(testId, passed, NULL);
    
    ///////////////////////////////////////////
    //testing SRL
    
    testId=mips_test_begin_test("srl");             //get the ID
    passed=0;
    
    err = mips_cpu_set_register(cpu, 8, 0x00000000ul);               //unsigned long
    if(err==0)
        err = mips_cpu_set_register(cpu, 9, 0xF0000F00ul);
    
    AND_instr = 0x1095102;    // R_type, 8 , 9 , 10, 00000, 100110
    invert_bytes(&AND_instr);
    if(err==0)
        err = mips_cpu_get_pc(cpu, &current_addr);
    
    if(err==0)
        err = mips_mem_write(mem,current_addr,4,(uint8_t*)&AND_instr);
    
    if(err==0){
        cout<<"CPU running srl"<<endl;
        err=mips_cpu_step(cpu);
    }
    if(err==0)           //if overflows occurs, test stops but desk register is still writen.
        err = (mips_error)(err | mips_cpu_get_register(cpu, 10, &got));         //stored in -14, Oxfffffff2,
    passed = (err == mips_Success) && (got== 0x0F0000F0);
    mips_test_end_test(testId, passed, NULL);
    
    
    ////////////////////////////////////
    //testing SLL
    
    testId=mips_test_begin_test("sll");             //get the ID
    passed=0;
    
    err = mips_cpu_set_register(cpu, 8, 0x00000000ul);               //unsigned long
    if(err==0)
        err = mips_cpu_set_register(cpu, 9, 0x0F000F00ul);       //rt
    
    AND_instr = 0x1095100;    // R_type, 8 , 9 , 10, 00000, 100110
    invert_bytes(&AND_instr);
    if(err==0)
        err = mips_cpu_get_pc(cpu, &current_addr);
    
    if(err==0)
        err = mips_mem_write(mem,current_addr,4,(uint8_t*)&AND_instr);
    
    if(err==0){
        cout<<"CPU running sll"<<endl;
        err=mips_cpu_step(cpu);
    }
    if(err==0){           //if overflows occurs, test stops but desk register is still writen.
        
        err = (mips_error)(err | mips_cpu_get_register(cpu, 10, &got));         //stored in -14, Oxfffffff2,
        
    }
    passed = (err == mips_Success) && (got== 0xF000F000);
    mips_test_end_test(testId, passed, NULL);
    
    
    ///////////////////////////////////////////
    //testing SRLV
    
    testId=mips_test_begin_test("srlv");             //get the ID
    passed=0;
    
    err = mips_cpu_set_register(cpu, 8, 0x00000004ul);               //unsigned long
    if(err==0)
        err = mips_cpu_set_register(cpu, 9, 0xF0000F00ul);
    
    AND_instr = 0x1095006;    // R_type, 8 , 9 , 10, 00000, 100110
    invert_bytes(&AND_instr);
    if(err==0)
        err = mips_cpu_get_pc(cpu, &current_addr);
    
    if(err==0)
        err = mips_mem_write(mem,current_addr,4,(uint8_t*)&AND_instr);
    
    if(err==0){
        cout<<"CPU running srlv"<<endl;
        err=mips_cpu_step(cpu);
    }
    if(err==0)           //if overflows occurs, test stops but desk register is still writen.
        err = (mips_error)(err | mips_cpu_get_register(cpu, 10, &got));         //stored in -14, Oxfffffff2,
    passed = (err == mips_Success) && (got== 0x0F0000F0);
    mips_test_end_test(testId, passed, NULL);
    
    ///////////////////////////////////////////
    //testing SLLV
    
    testId=mips_test_begin_test("sllv");             //get the ID
    passed=0;
    
    err = mips_cpu_set_register(cpu, 8, 0x00000004ul);               //unsigned long
    if(err==0)
        err = mips_cpu_set_register(cpu, 9, 0x0F000F00ul);
    
    AND_instr = 0x01095004;    // R_type, 8 , 9 , 10, 00000, 100110
    invert_bytes(&AND_instr);
    if(err==0)
        err = mips_cpu_get_pc(cpu, &current_addr);
    
    if(err==0)
        err = mips_mem_write(mem,current_addr,4,(uint8_t*)&AND_instr);
    
    if(err==0){
        cout<<"CPU running sllv"<<endl;
        err=mips_cpu_step(cpu);
    }
    if(err==0)           //if overflows occurs, test stops but desk register is still writen.
        err = (mips_error)(err | mips_cpu_get_register(cpu, 10, &got));         //stored in -14, Oxfffffff2,
    passed = (err == mips_Success) && (got== 0xF000F000);
    mips_test_end_test(testId, passed, NULL);
    
    ///////////////////////////////////////////
    //testing ANDI
    
    
    testId=mips_test_begin_test("andi");             //get the ID
    passed=0;
    
    err = mips_cpu_set_register(cpu,2, 0x000FFFF0ul);     //unsigned long
    if(err==0)
    
    AND_instr = 0x302200F0;    // I_type, 1 , 2 , 16-bit imm
    invert_bytes(&AND_instr);
    if(err==0)
        err = mips_cpu_get_pc(cpu, &current_addr);
    
    if(err==0)
        err = mips_mem_write(mem,current_addr,4,(uint8_t*)&AND_instr);
    
    if(err==0){
        cout<<"CPU running andi"<<endl;
        err=mips_cpu_step(cpu);
    }
    
    if(err==0)           //if overflows occurs, test stops but desk register is still writen.
        err = (mips_error)(err | mips_cpu_get_register(cpu, 1, &got));         //stored in -14, Oxfffffff2,
    passed = (err == mips_Success) && (got== 0x000000F0);
    mips_test_end_test(testId, passed, NULL);
    
    ///////////////////////////////////////////
    
    //testing ORI
    
    
    testId=mips_test_begin_test("ori");             //get the ID
    passed=0;
    
    err = mips_cpu_set_register(cpu,2, 0x000FFFF0ul);     //unsigned long
    if(err==0)
        AND_instr = 0x3422000F;    // I_type, 1 , 2 , 16-bit imm
    invert_bytes(&AND_instr);
    if(err==0)
        err = mips_cpu_get_pc(cpu, &current_addr);
    
    if(err==0)
        err = mips_mem_write(mem,current_addr,4,(uint8_t*)&AND_instr);
    
    if(err==0){
        cout<<"CPU running ori"<<endl;
        err=mips_cpu_step(cpu);
    }
    if(err==0)           //if overflows occurs, test stops but desk register is still writen.
        err = (mips_error)(err | mips_cpu_get_register(cpu, 1, &got));         //stored in -14, Oxfffffff2,
    
    passed = (err == mips_Success) && (got== 0x000FFFFF);
    
    mips_test_end_test(testId, passed, NULL);
    
    
    ///////////////////////////////////////////
    
    //testing XORI
    
    
    testId=mips_test_begin_test("xori");             //get the ID
    passed=0;
    
    err = mips_cpu_set_register(cpu,2, 0x000FFFFFul);     //unsigned long
    if(err==0)
        AND_instr = 0x3822000F;    // I_type, 1 , 2 , 16-bit imm
    invert_bytes(&AND_instr);
    
    if(err==0)
        err = mips_cpu_get_pc(cpu, &current_addr);
    
    if(err==0)
        err = mips_mem_write(mem,current_addr,4,(uint8_t*)&AND_instr);
    
    if(err==0){
        cout<<"CPU running xori"<<endl;
        err=mips_cpu_step(cpu);
    }
    if(err==0)           //if overflows occurs, test stops but desk register is still writen.
        err = (mips_error)(err | mips_cpu_get_register(cpu, 1, &got));         //stored in -14, Oxfffffff2,
    
    passed = (err == mips_Success) && (got== 0x000FFFF0);
    
    mips_test_end_test(testId, passed, NULL);
    
    ///////////////////////////////////////////
    //testing ADDI
    
    
    testId=mips_test_begin_test("addi");             //get the ID
    passed=0;
    
    err = mips_cpu_set_register(cpu,2, 0xFul);     //unsigned long
    if(err==0)
        
        AND_instr = 0x20220001;    // I_type, 1 , 2 , 16-bit imm
    invert_bytes(&AND_instr);
    if(err==0)
        err = mips_cpu_get_pc(cpu, &current_addr);
    
    if(err==0)
        err = mips_mem_write(mem,current_addr,4,(uint8_t*)&AND_instr);
    
    if(err==0){
        cout<<"CPU running addi"<<endl;
        err=mips_cpu_step(cpu);
    }
    if(err==0)           //if overflows occurs, test stops but desk register is still writen.
        err = (mips_error)(err | mips_cpu_get_register(cpu, 1, &got));         //stored in -14, Oxfffffff2,
    
    passed = (err == mips_Success) && (got== 0x00000010);
    
    mips_test_end_test(testId, passed, NULL);
    
    ///////////////////////////////////////////
    //testing ADDIU
    
    
    testId=mips_test_begin_test("addiu");             //get the ID
    passed=0;
    
    err = mips_cpu_set_register(cpu,2, 0x7FFFFFFFul);     //unsigned long
    if(err==0)
        
        AND_instr = 0x24220001;    // I_type, 1 , 2 , 16-bit imm
    invert_bytes(&AND_instr);
    if(err==0)
        err = mips_cpu_get_pc(cpu, &current_addr);
    
    if(err==0)
        err = mips_mem_write(mem,current_addr,4,(uint8_t*)&AND_instr);
    
    if(err==0){
        cout<<"CPU running addiu"<<endl;
        err=mips_cpu_step(cpu);
    }
    if(err==0)           //if overflows occurs, test stops but desk register is still writen.
        err = (mips_error)(err | mips_cpu_get_register(cpu, 1, &got));         //stored in -14, Oxfffffff2,
    
    passed = (err == mips_Success) && (got== 0x80000000);
    
    mips_test_end_test(testId, passed, NULL);
    
    
    
    ///////////////////////////////////////////
    //testing LUI
    
    
    testId=mips_test_begin_test("lui");             //get the ID
    passed=0;
    
    err = mips_cpu_set_register(cpu,2, 0x7FFFFFFFul);     //unsigned long
    if(err==0)
        
        AND_instr = 0x3C020030;    // I_type, 0 , 2 , 16-bit imm ==     0011 0000
    invert_bytes(&AND_instr);
    if(err==0)
        err = mips_cpu_get_pc(cpu, &current_addr);
    
    if(err==0)
        err = mips_mem_write(mem,current_addr,4,(uint8_t*)&AND_instr);
    
    if(err==0){
        cout<<"CPU running lui"<<endl;
        err=mips_cpu_step(cpu);
    }
    if(err==0)           //if overflows occurs, test stops but desk register is still writen.
        err = (mips_error)(err | mips_cpu_get_register(cpu, 2, &got));         //stored in -14, Oxfffffff2,
    
    passed = (err == mips_Success) && (got== 0x00300000);
    
    mips_test_end_test(testId, passed, NULL);
    
    
    
    
    ///////////////////////////////////////////
    //testing SW
    
    
    testId=mips_test_begin_test("sw");             //get the ID
    passed=0;
    
    err = mips_cpu_set_register(cpu,2, 0x7FFFFFFFul);     // rt, M[rs+imm] <- rt
    if(err==0)
        err = mips_cpu_set_register(cpu, 1, 0x0000FFFFul);  //rs

    AND_instr = 0xAC220001;    // I_type, 1 , 2 , 16-bit imm
    invert_bytes(&AND_instr);
    if(err==0)
        err = mips_cpu_get_pc(cpu, &current_addr);
    
    if(err==0)
        err = mips_mem_write(mem,current_addr,4,(uint8_t*)&AND_instr);
    
    if(err==0){
        cout<<"CPU running sw"<<endl;
        err=mips_cpu_step(cpu);
    }
    if(err==0){           //if overflows occurs, test stops but desk register is still writen.
        err = (mips_error)(err |  mips_mem_read(mem, 0x00010000, 4, (uint8_t*)&got));         //stored in -14, Oxfffffff2,

    }

    
    passed = (err == mips_Success) && (got== 0x7FFFFFFF);
    
    mips_test_end_test(testId, passed, NULL);
                           
                           
    
    ///////////////////////////////////////////
    //testing LW
    
    
    testId=mips_test_begin_test("lw");             //get the ID
    passed=0;
    
    err = mips_cpu_set_register(cpu,2, 0x7FFFFFFFul);     // rt, M[rs+imm] -> rt
    if(err==0)
        err = mips_cpu_set_register(cpu, 1, 0x0000FFF0ul);  //rs

    if(err==0){
        uint32_t mem_data = 0x000F0000;
        uint32_t mem_da = 0x00000FA0;       //must be 4's multiples 
        err = mips_mem_write(mem, mem_da, 4, (uint8_t*)&mem_data);              //???????

    }
    AND_instr = 0x8C220001;    // I_type, 1 , 2 , 16-bit imm
    invert_bytes(&AND_instr);
    if(err==0)
        err = mips_cpu_get_pc(cpu, &current_addr);

    if(err==0)
        err = mips_mem_write(mem,current_addr,4,(uint8_t*)&AND_instr);

    if(err==0){
        cout<<"CPU running lw"<<endl;
        err=mips_cpu_step(cpu);
    }
    
    
    if(err==0){           //if overflows occurs, test stops but desk register is still writen.

        err = (mips_error)(err |  mips_cpu_get_register(cpu, 2, &got));         //stored in -14, Oxfffffff2,

    }
    
    passed = (err == mips_Success) && (got== 0x7FFFFFFF);
    
    mips_test_end_test(testId, passed, NULL);
    
    /////////////////////////////////////////////////////********Branch*******************************
    //testing BEQ
    
    
    testId=mips_test_begin_test("beq");             //get the ID
    passed=0;
    
    err = mips_cpu_set_register(cpu,1, 0x7FFFFFFFul);     //unsigned long
    if(err==0)
        err = mips_cpu_set_register(cpu,2, 0x7FFFFFFFul);     //unsigned long
    
    if(err==0)
        AND_instr = 0x102201BC;    // I_type, 0 , 2 , 16-bit imm ==     1 1011 1100, (444)
    invert_bytes(&AND_instr);
    if(err==0)
        err = mips_cpu_get_pc(cpu, &current_addr);
//    cout<<"check PC"<<current_addr<<endl;
    if(err==0)
        err = mips_mem_write(mem,current_addr,4,(uint8_t*)&AND_instr);
    
    uint32_t nPC;
    err = mips_cpu_get_pcN(cpu, &nPC);

    if(err==0){
        cout<<"CPU running beq"<<endl;
        err=mips_cpu_step(cpu);
    }
    if(err==0)           //if overflows occurs, test stops but desk register is still writen.
        err = (mips_error)(err | mips_cpu_get_pcN(cpu, &branched_addr));
    
    passed = (err == mips_Success) && (branched_addr== (nPC+1776));  //imm<<2,(1776);
    
    
//    err = mips_cpu_get_pc(cpu, &current_addr);
//    cout<<"check PC"<<current_addr<<endl;
    
    mips_test_end_test(testId, passed, NULL);
    
    
    //////////////////////////////////////////////////////////////////////////////////////
    //testing BNE
    
    
    testId=mips_test_begin_test("bne");             //get the ID
    passed=0;
    
    err = mips_cpu_set_register(cpu,1, 0x7FFFFFFFul);     //rs
    if(err==0)
        err = mips_cpu_set_register(cpu,2, 0x7FFFFFF1ul);     //rt
    
    if(err==0)
        AND_instr = 0x142201BC;    // I_type, 0 , 2 , 16-bit imm ==     1 1011 1100, (444)
    invert_bytes(&AND_instr);
    if(err==0)
        err = mips_cpu_get_pc(cpu, &current_addr);
    //    cout<<"check PC"<<current_addr<<endl;
    if(err==0)
        err = mips_mem_write(mem,current_addr,4,(uint8_t*)&AND_instr);
    
    if(err == 0)
        err = mips_cpu_get_pcN(cpu, &nPC);

    if(err==0){
        cout<<"CPU running bne"<<endl;
        err=mips_cpu_step(cpu);
    }
    if(err==0)           //if overflows occurs, test stops but desk register is still writen.
        err = (mips_error)(err | mips_cpu_get_pcN(cpu, &branched_addr));
    
    passed = (err == mips_Success) && (branched_addr== (nPC+1776));   //imm<<2,(1776);
    
    
    //    err = mips_cpu_get_pc(cpu, &current_addr);
    //    cout<<"check PC"<<current_addr<<endl;
    
    mips_test_end_test(testId, passed, NULL);

    
    //////////////////////////////////////////////////////////////////////////////////////*****************
    //testing BGEZ
    
    
    testId=mips_test_begin_test("bgez");             //get the ID
    passed=0;
    
    err = mips_cpu_set_register(cpu,1, 0x70000000ul);     //rs
    if(err==0)
        AND_instr = 0x042101BC;    // I_type, 0 , 2 , 16-bit imm ==     1 1011 1100, (444)
    invert_bytes(&AND_instr);
    if(err==0)
        err = mips_cpu_get_pc(cpu, &current_addr);
    //    cout<<"check PC"<<current_addr<<endl;
    if(err==0)
        err = mips_mem_write(mem,current_addr,4,(uint8_t*)&AND_instr);
    
    if(err==0)
        err = mips_cpu_get_pcN(cpu, &nPC);

    
    if(err==0){
        cout<<"CPU running bgez"<<endl;
        err=mips_cpu_step(cpu);
    }
    
    if(err==0){           //if overflows occurs, test stops but desk register is still writen.
        err = (mips_error)(err | mips_cpu_get_pc(cpu, &branched_addr));
        
        
    }
    
    if (err==0)
        passed = (err == mips_Success) && (branched_addr== (nPC+1776));   //imm<<2,(1776);
    
    
    //    err = mips_cpu_get_pc(cpu, &current_addr);
    //    cout<<"check PC"<<current_addr<<endl;
    
    mips_test_end_test(testId, passed, NULL);
    
    
    //////////////////////////////////////////////////////////////////////////////////////
    
    //testing BGEZAL
    
    
    testId=mips_test_begin_test("bgezal");             //get the ID
    passed=0;
    
    err = mips_cpu_set_register(cpu,1, 0x70000000ul);     //rs
    if(err==0)
        AND_instr = 0x043101BC;    // I_type, 0 , 2 , 16-bit imm ==     1 1011 1100, (444)
    invert_bytes(&AND_instr);
    if(err==0)
        err = mips_cpu_get_pc(cpu, &current_addr);
    //    cout<<"check PC"<<current_addr<<endl;
    if(err==0)
        err = mips_mem_write(mem,current_addr,4,(uint8_t*)&AND_instr);
    
    if(err==0)
        err = mips_cpu_get_pcN(cpu, &nPC);
    
    
    if(err==0){
        cout<<"CPU running bgezal"<<endl;
        err=mips_cpu_step(cpu);
    }
    
    if(err==0){
        err = (mips_error)(err | mips_cpu_get_pc(cpu, &branched_addr));
        
    }
    
    if(err==0){
        err = (mips_error)(err | mips_cpu_get_register(cpu, 31, &return_addr));
        
    }
    if (err==0)
        passed = (err == mips_Success) &&( (branched_addr== (nPC+1776)) && (return_addr == (current_addr + 8 )));   //imm<<2,(1776);
    
    
    //    err = mips_cpu_get_pc(cpu, &current_addr);
    //    cout<<"check PC"<<current_addr<<endl;
    
    mips_test_end_test(testId, passed, NULL);
    
    
    //////////////////////////////////////////////////////////////////////////////////////
    //testing BGTZ
    
    
    testId=mips_test_begin_test("bgtz");             //get the ID
    passed=0;
    
    err = mips_cpu_set_register(cpu,1, 0x1ul);     //rs
    if(err==0)
        AND_instr = 0x1C2101BC;    // I_type, 0 , 2 , 16-bit imm ==     1 1011 1100, (444)
    invert_bytes(&AND_instr);
    if(err==0)
        err = mips_cpu_get_pc(cpu, &current_addr);
    //    cout<<"check PC"<<current_addr<<endl;
    if(err==0)
        err = mips_mem_write(mem,current_addr,4,(uint8_t*)&AND_instr);
    
    
    if(err==0)
        err = mips_cpu_get_pcN(cpu, &nPC);
    
    if(err==0){
        cout<<"CPU running bgtz"<<endl;
        err=mips_cpu_step(cpu);
    }
    if(err==0)           //if overflows occurs, test stops but desk register is still writen.
        err = (mips_error)(err | mips_cpu_get_pc(cpu, &branched_addr));
    
    if (err==0)
        passed = (err == mips_Success) && (branched_addr== (nPC+1776));
    
    //    err = mips_cpu_get_pc(cpu, &current_addr);
    //    cout<<"check PC"<<current_addr<<endl;
    
    mips_test_end_test(testId, passed, NULL);
    
    
    //////////////////////////////////////////////////////////////////////////////////////
    //testing BLEZ
    
    
    testId=mips_test_begin_test("blez");             //get the ID
    passed=0;
    
    err = mips_cpu_set_register(cpu,1, 0x80000000ul);     //rs
    if(err==0)
        AND_instr = 0x182001BC;    // I_type, 0 , 2 , 16-bit imm ==     1 1011 1100, (444)
    invert_bytes(&AND_instr);
    if(err==0)
        err = mips_cpu_get_pc(cpu, &current_addr);
    //    cout<<"check PC"<<current_addr<<endl;
    if(err==0)
        err = mips_mem_write(mem,current_addr,4,(uint8_t*)&AND_instr);
    if(err==0)
        err = mips_cpu_get_pcN(cpu, &nPC);
    
    if(err==0){
        cout<<"CPU running blez"<<endl;
        err=mips_cpu_step(cpu);
    }
    if(err==0){           //if overflows occurs, test stops but desk register is still writen.
        err = (mips_error)(err | mips_cpu_get_pc(cpu, &branched_addr));
    }
    
    if (err==0)
        passed = (err == mips_Success) && (branched_addr== (nPC+1776));
    
    //    err = mips_cpu_get_pc(cpu, &current_addr);
    //    cout<<"check PC"<<current_addr<<endl;
    
    mips_test_end_test(testId, passed, NULL);
    
    
    //////////////////////////////////////////////////////////////////////////////////////
    //testing BLTZ
    
    
    testId=mips_test_begin_test("bltz");             //get the ID
    passed=0;
    
    err = mips_cpu_set_register(cpu,1, 0x80000000ul);     //rs
    if(err==0)
        AND_instr = 0x042001BC;    // I_type, 0 , 2 , 16-bit imm ==     1 1011 1100, (444)
    invert_bytes(&AND_instr);
    if(err==0)
        err = mips_cpu_get_pc(cpu, &current_addr);
    //    cout<<"check PC"<<current_addr<<endl;
    if(err==0)
        err = mips_mem_write(mem,current_addr,4,(uint8_t*)&AND_instr);
    
    if(err==0)
        err = mips_cpu_get_pcN(cpu, &nPC);
    
    if(err==0){
        cout<<"CPU running bltz"<<endl;
        err=mips_cpu_step(cpu);
    }
    if(err==0)           //if overflows occurs, test stops but desk register is still writen.
        err = (mips_error)(err | mips_cpu_get_pc(cpu, &branched_addr));
    
    if (err==0)
        passed = (err == mips_Success) && (branched_addr== (nPC+1776));
    
    //    err = mips_cpu_get_pc(cpu, &current_addr);
    //    cout<<"check PC"<<current_addr<<endl;
    
    mips_test_end_test(testId, passed, NULL);
    
    
    //////////////////////////////////////////////////////////////////////////////////////
    //testing BLTZAL
    
    
    testId=mips_test_begin_test("bltzal");             //get the ID
    passed=0;
    
    err = mips_cpu_set_register(cpu,1, 0x80000000ul);     //rs
    if(err==0)
        AND_instr = 0x043001BC;    // I_type, 0 , 2 , 16-bit imm ==     1 1011 1100, (444)
    invert_bytes(&AND_instr);
    
    if(err==0)
        err = mips_cpu_get_pc(cpu, &current_addr);
    //    cout<<"check PC"<<current_addr<<endl;
    if(err==0)
        err = mips_mem_write(mem,current_addr,4,(uint8_t*)&AND_instr);
    
    if(err==0)
        err = mips_cpu_get_pcN(cpu, &nPC);
    
    
    if(err==0){
        cout<<"CPU running bgezal"<<endl;
        err=mips_cpu_step(cpu);
    }
    
    if(err==0){
        err = (mips_error)(err | mips_cpu_get_pc(cpu, &branched_addr));
        
    }
    
    if(err==0){
        err = (mips_error)(err | mips_cpu_get_register(cpu, 31, &return_addr));
        
    }
    if (err==0)
        passed = (err == mips_Success) &&( (branched_addr== (nPC+1776)) && (return_addr == (current_addr + 8 )));   //imm<<2,(1776);    mips_test_end_test(testId, passed, NULL);
    
    
    mips_test_end_test(testId, passed, NULL);
    
    //////////////////////////////////////////////////////////////////////////////////////
    //testing SLTI    
    
    testId=mips_test_begin_test("slti");             //get the ID
    passed=0;
    
    err = mips_cpu_set_register(cpu, 1, 0x00000001ul);               //unsigned long
    if(err==0)
        err = mips_cpu_set_register(cpu, 2, 0x00000000ul);
    
    AND_instr = 0x28228000;    // I-type  : 001011 00001 00010 1000 0000 1111 0000
    invert_bytes(&AND_instr);
    if(err==0)
        err = mips_cpu_get_pc(cpu, &current_addr);
    
    if(err==0)
        err = mips_mem_write(mem,current_addr,4,(uint8_t*)&AND_instr);
    
    if(err==0){
        cout<<"CPU running:SLTI"<<endl;
        err=mips_cpu_step(cpu);
    }
    if(err==0)           //if overflows occurs, test stops but desk register is still writen.
        err = (mips_error)(err | mips_cpu_get_register(cpu, 2, &got));         //stored in -14, Oxfffffff2,
    passed = (err == mips_Success) && (got== 0x00000000);       //1 < -XXXXX
    mips_test_end_test(testId, passed, NULL);
    
    
    ////////////////////////////////////////////////////////////////////////
    //testing SLTIU
    
    testId=mips_test_begin_test("sltiu");             //get the ID
    passed=0;
    
    err = mips_cpu_set_register(cpu, 1, 0x80000000ul);               //unsigned long
    if(err==0)
        err = mips_cpu_set_register(cpu, 2, 0x00000000ul);
    
    AND_instr = 0x2C228000;    // I-type  : 001011 00001 00010 1000 0000 1111 0000
    invert_bytes(&AND_instr);
    if(err==0)
        err = mips_cpu_get_pc(cpu, &current_addr);
    
    if(err==0)
        err = mips_mem_write(mem,current_addr,4,(uint8_t*)&AND_instr);
    
    if(err==0){
        cout<<"CPU running sltiu"<<endl;
        err=mips_cpu_step(cpu);
    }
    if(err==0){           //if overflows occurs, test stops but desk register is still writen.
        err = (mips_error)(err | mips_cpu_get_register(cpu, 2, &got));
    }
    
    passed = (err == mips_Success) && (got== 0x00000001);
    mips_test_end_test(testId, passed, NULL);
    
    
    ////////////////////////////////////////////////////////////////////////
    //testing DIV function
    
    testId=mips_test_begin_test("div");             //get the ID
    passed=0;
    
    err = mips_cpu_set_register(cpu, 7, 0xFFFFFFFBul);               //-5
    if(err==0)
        err = mips_cpu_set_register(cpu, 8, 0xFFFFFFFEul);          //-2
    
    AND_instr = 0x00E8001A;    // R_type, reg7, reg8
    invert_bytes(&AND_instr);
    if(err==0)
        err = mips_cpu_get_pc(cpu, &current_addr);
    
    if(err==0)
        err = mips_mem_write(mem,current_addr,4,(uint8_t*)&AND_instr);
    
    
    if(err==0){
        cout<<"CPU running div"<<endl;
        err=mips_cpu_step(cpu);
    }
    
    if(err==0){
        get_LO(cpu, &reg_lo);
        get_HI(cpu, &reg_hi);
    }
    
    passed = (err == mips_Success) && ((reg_lo == 0x2) && (reg_hi == 0xFFFFFFFF));
    mips_test_end_test(testId, passed, NULL);
    
    //I have tested 5/2, -5/2, -4/2, -5/-2
    
    //////////////////////////////////////////////////////////////////////
    
    //testing DIVU function
    
    testId=mips_test_begin_test("divu");             //get the ID
    passed=0;
    
    err = mips_cpu_set_register(cpu, 7, 0x80000000ul);               //2147483648
    if(err==0)
        err = mips_cpu_set_register(cpu, 8, 0x2ul);          //2
    
    AND_instr = 0x00E8001B;    // R_type, reg7, reg8
    invert_bytes(&AND_instr);
    if(err==0)
        err = mips_cpu_get_pc(cpu, &current_addr);
    
    if(err==0)
        err = mips_mem_write(mem,current_addr,4,(uint8_t*)&AND_instr);
    
    
    if(err==0){
        cout<<"CPU running divu"<<endl;
        err=mips_cpu_step(cpu);
    }
    
    if(err==0){
        get_LO(cpu, &reg_lo);
        get_HI(cpu, &reg_hi);
    }
    
    passed = (err == mips_Success) && ((reg_lo == 1073741824) && (reg_hi == 0));
    mips_test_end_test(testId, passed, NULL);
    
    //I have tested 5/2, -5/2, -4/2, -5/-2
    
    ////////////////////////////////////////////////////////////////////
    //testing MULT function
    
    testId=mips_test_begin_test("mult");             //get the ID
    passed=0;
    
    err = mips_cpu_set_register(cpu, 7, -2);               //2147483648
    if(err==0)
        err = mips_cpu_set_register(cpu, 8, 3);          //2
    
    AND_instr = 0x00E80018;    // R_type, reg7, reg8
    invert_bytes(&AND_instr);
    if(err==0)
        err = mips_cpu_get_pc(cpu, &current_addr);
    
    if(err==0)
        err = mips_mem_write(mem,current_addr,4,(uint8_t*)&AND_instr);
    
    
    if(err==0){
        cout<<"CPU running mult"<<endl;
        err=mips_cpu_step(cpu);
    }
    
    if(err==0){
        get_LO(cpu, &reg_lo);
        get_HI(cpu, &reg_hi);
    }
    
    passed = (err == mips_Success) && ((reg_lo == 0xfffffffa) && (reg_hi == 0xffffffff));
    mips_test_end_test(testId, passed, NULL);
    
    
    ////////////////////////////////////////////////////////////////////
    //testing MULTU function
    
    testId=mips_test_begin_test("multu");             //get the ID
    passed=0;
    
    err = mips_cpu_set_register(cpu, 7, 3);               //2147483648
    if(err==0)
        err = mips_cpu_set_register(cpu, 8, 2);          //2
    
    AND_instr = 0x00E80019;    // R_type, reg7, reg8
    invert_bytes(&AND_instr);
    if(err==0)
        err = mips_cpu_get_pc(cpu, &current_addr);
    
    if(err==0)
        err = mips_mem_write(mem,current_addr, 4, (uint8_t*)&AND_instr);
    
    
    if(err==0){
        cout<<"CPU running multu"<<endl;
        err=mips_cpu_step(cpu);
    }
    //run MFLO
        AND_instr = 0x00007012;    // R_type  000000 00000 00000 1110 00000 010010
        invert_bytes(&AND_instr);
        if(err==0)
            err = mips_cpu_get_pc(cpu, &current_addr);
    
        if(err==0)
            err = mips_mem_write(mem,current_addr,4,(uint8_t*)&AND_instr);
    
        if(err==0){
            cout<<"CPU running mflo"<<endl;
            err=mips_cpu_step(cpu);
        }
    
        if(err==0){
            err = mips_cpu_get_register(cpu, 14, &mflo);               //2147483648
        }
    //run MFHI
        AND_instr = 0x0000F810;    // R_type  000000 00000 00000 11111 00000 010000
        invert_bytes(&AND_instr);
        if(err==0)
            err = mips_cpu_get_pc(cpu, &current_addr);
    
    
    
        if(err==0)
            err = mips_mem_write(mem,current_addr,4,(uint8_t*)&AND_instr);
    
        if(err==0){
            cout<<"CPU running mflo"<<endl;
            err=mips_cpu_step(cpu);
        }
    
        if(err==0){
            err = mips_cpu_get_register(cpu, 15, &mfhi);               //2147483648
        }
    
    passed = (err == mips_Success) && ( (mflo == 0x6) && (mfhi== 0x0) );
    mips_test_end_test(testId, passed, NULL);
    
    testId=mips_test_begin_test("mflo");             //The above thing proves MFLO & MFHI are working
    passed =1;
    mips_test_end_test(testId, passed, NULL);
    
    testId=mips_test_begin_test("mfhi");             //The above thing proves MFLO & MFHI are working
    passed =1;
    mips_test_end_test(testId, passed, NULL);


    
//    //nested testing MFLO
//    
//    testId=mips_test_begin_test("mflo");             //get the ID
//    passed=0;
//    
//    
//    AND_instr = 0x0000F012;    // R_type  000000 00000 00000 1110 00000 010010
//    invert_bytes(&AND_instr);
//    if(err==0)
//        err = mips_cpu_get_pc(cpu, &current_addr);
//    
//    
//    
//    if(err==0)
//        err = mips_mem_write(mem,current_addr,4,(uint8_t*)&AND_instr);
//    
//    if(err==0){
//        cout<<"CPU running mflo"<<endl;
//        err=mips_cpu_step(cpu);
//    }
//    
//    if(err==0){
//        err = mips_cpu_get_register(cpu, 14, &mflo);               //2147483648
//        
//    }
//    
//    
//    passed = (err == mips_Success) && (mflo == 0x6);
//    if(passed == 1)
//        mflo_sucess = 1;
//    mips_test_end_test(testId, passed, NULL);
//    
//    
//    
//    //nested testing MFLO
//    
//    testId=mips_test_begin_test("mfhi");             //get the ID
//    passed=0;
//    
//    
//    AND_instr = 0x0000F010;    // R_type  000000 00000 00000 11110 00000 010000
//    invert_bytes(&AND_instr);
//    if(err==0)
//        err = mips_cpu_get_pc(cpu, &current_addr);
//    
//    
//    
//    if(err==0)
//        err = mips_mem_write(mem,current_addr,4,(uint8_t*)&AND_instr);
//    
//    if(err==0){
//        cout<<"CPU running mflo"<<endl;
//        err=mips_cpu_step(cpu);
//    }
//    
//    if(err==0){
//        err = mips_cpu_get_register(cpu, 14, &mfhi);               //2147483648
//        
//    }
//    
//    
//    passed = (err == mips_Success) && (mfhi == 0x0);
//    if(passed == 1)
//        mfhi_sucess = 1;
//    mips_test_end_test(testId, passed, NULL);
//    
//    
//    passed = (err == mips_Success) && (mfhi_sucess && mflo_sucess);

    
    
    ////////////////////////////////////////////////////////////////////
    //testing LB
    
    testId=mips_test_begin_test("lb");             //get the ID
    passed=0;
    
    err = mips_cpu_set_register(cpu, 2, 0x00009C40ul);               //base  = 40000
    if(err==0)
        err = mips_cpu_set_register(cpu, 3, 0x00000000ul);
    
    AND_instr = 0x80430FA1;    // I-type  : 100000 00010 00011 00000 00000 010000, offset = 4001
    invert_bytes(&AND_instr);
    if(err==0)
        err = mips_cpu_get_pc(cpu, &current_addr);
    
    if(err==0)
        err = mips_mem_write(mem,current_addr,4,(uint8_t*)&AND_instr);
    
    data_32 = 0x00008800;
    invert_bytes(&data_32);
    data_mem = 0x00009C40 + 0xFA0; // (must be 4's multiplies)
    
    if(err==0)
        err = mips_mem_write(mem,data_mem,4,(uint8_t*)&data_32);
    
    if(err==0){
        cout<<"CPU running lb "<<endl;
        err=mips_cpu_step(cpu);
    }
    if(err==0){           //if overflows occurs, test stops but desk register is still writen.
        err = (mips_error)(err | mips_cpu_get_register(cpu, 3, &got));
    }
    
    passed = (err == mips_Success) && ( got == 0xFFFFFF88);
    mips_test_end_test(testId, passed, NULL);
    
    
    ////////////////////////////////////////////////////////////////////////
    //testing LBU
    
    testId=mips_test_begin_test("lbu");             //get the ID
    passed=0;
    
    err = mips_cpu_set_register(cpu, 2, 0x00009C40ul);               //base  = 40000
    if(err==0)
        err = mips_cpu_set_register(cpu, 3, 0x00000000ul);
    
    AND_instr = 0x90430FA1;    // I-type  : 100000 00010 00011 00000 00000 010000, offset = 4001
    invert_bytes(&AND_instr);
    if(err==0)
        err = mips_cpu_get_pc(cpu, &current_addr);
    
    if(err==0)
        err = mips_mem_write(mem,current_addr,4,(uint8_t*)&AND_instr);
    
    data_32 = 0x00008800;
    invert_bytes(&data_32);
    data_mem = 0x00009C40 + 0xFA0; // (must be 4's multiplies)
    
    if(err==0)
        err = mips_mem_write(mem,data_mem,4,(uint8_t*)&data_32);
    
    if(err==0){
        cout<<"CPU running lbu "<<endl;
        err=mips_cpu_step(cpu);
    }
    if(err==0){           //if overflows occurs, test stops but desk register is still writen.
        err = (mips_error)(err | mips_cpu_get_register(cpu, 3, &got));
    }
    
    passed = (err == mips_Success) && ( got == 0x88);
    
    mips_test_end_test(testId, passed, NULL);
    
    
    ////////////////////////////////////////////////////////////////////////
    //testing SB
    
    testId=mips_test_begin_test("sb");             //get the ID
    passed=0;
    
    err = mips_cpu_set_register(cpu, 2, 0x00009C40ul);               //base  = 40000
    if(err==0)
        err = mips_cpu_set_register(cpu, 3, 0x07891022ul);
    
    AND_instr = 0xA0430FA1;    // I-type  : 101000 00010 00011 00000 00000 010000, offset = 4001
    invert_bytes(&AND_instr);
    if(err==0)
        err = mips_cpu_get_pc(cpu, &current_addr);
    
    if(err==0)
        err = mips_mem_write(mem,current_addr,4,(uint8_t*)&AND_instr);
    
    data_32 = 0x66778899;
    invert_bytes(&data_32);
    data_mem = 0x00009C40 + 0xFA0; // (must be 4's multiplies)
    
    if(err==0)
        err = mips_mem_write(mem,data_mem,4,(uint8_t*)&data_32);
    
    if(err==0){
        cout<<"CPU running sb "<<endl;
        err=mips_cpu_step(cpu);
    }
    if(err==0){           //if overflows occurs, test stops but desk register is still writen.
        err = (mips_error)(err | mips_mem_read(mem,data_mem,4,(uint8_t*)&got));
    }
    if(err==0)
        passed = (err == mips_Success) && ( got == 0x99227766);
    
    mips_test_end_test(testId, passed, NULL);
    
    
    ////////////////////////////////////////////////////////////////////////
    
    //testing SH
    
    testId=mips_test_begin_test("sh");             //get the ID
    passed=0;
    
    err = mips_cpu_set_register(cpu, 2, 0x00009C40ul);               //base  = 40000
    if(err==0)
        err = mips_cpu_set_register(cpu, 3, 0x07891022ul);
    
    AND_instr = 0xA4430FA1;    // I-type  : 101000 00010 00011 00000 00000 010000, offset = 4001
    invert_bytes(&AND_instr);
    if(err==0)
        err = mips_cpu_get_pc(cpu, &current_addr);
    
    if(err==0)
        err = mips_mem_write(mem,current_addr,4,(uint8_t*)&AND_instr);
    
    data_32 = 0x66778899;
    invert_bytes(&data_32);
    data_mem = 0x00009C40 + 0xFA0; // (must be 4's multiplies)
    
    if(err==0)
        err = mips_mem_write(mem,data_mem,4,(uint8_t*)&data_32);        //0x99887766 in mem
    
    if(err==0){
        cout<<"CPU running sh "<<endl;
        err=mips_cpu_step(cpu);
    }
    if(err==0){           //if overflows occurs, test stops but desk register is still writen.
        err = (mips_error)(err | mips_mem_read(mem,data_mem,4,(uint8_t*)&got));
    }
    if(err==0)
        passed = (err == mips_Success) && ( got == 0x10227766);
    
    mips_test_end_test(testId, passed, NULL);
    
    
    ////////////////////////////////////////////////////////////////////////
    
    ////////////////////////////////////////////////////////////////////////
    
    //testing LWL
    
    testId=mips_test_begin_test("lwl");             //get the ID
    passed=0;
    
    err = mips_cpu_set_register(cpu, 2, 0x00009C40ul);               //base  = 40000
    if(err==0)
        err = mips_cpu_set_register(cpu, 3, 0x00000000ul);
    
    AND_instr = 0x88430FA1;    // I-type  : 101000 00010 00011 00000 00000 010000, offset = 4001
    invert_bytes(&AND_instr);
    if(err==0)
        err = mips_cpu_get_pc(cpu, &current_addr);
    
    if(err==0)
        err = mips_mem_write(mem,current_addr,4,(uint8_t*)&AND_instr);
    
    data_32 = 0x66778899;
//    data_32_pre = 0x11223344;
    data_32_post = 0xAABBCCDD;
    invert_bytes(&data_32);
//    invert_bytes(&data_32_pre);
    invert_bytes(&data_32_post);


    
    data_mem = 0x00009C40 + 0xFA4; // (must be 4's multiplies)
    if(err==0)
        err = mips_mem_write(mem,data_mem,4,(uint8_t*)&data_32_post);        //0xDDCCBBAA in mem
    
    data_mem = 0x00009C40 + 0xFA0; // (must be 4's multiplies)
    if(err==0)
        err = mips_mem_write(mem,data_mem,4,(uint8_t*)&data_32);        //0x99887766 in mem
   
//    data_mem = 0x00009C40 + 0xF9C; // (must be 4's multiplies)
//    if(err==0)
//        err = mips_mem_write(mem,data_mem,4,(uint8_t*)&data_32_pre);        //0x44332211 in mem
    
    if(err==0){
        cout<<"CPU running lwl "<<endl;
        err=mips_cpu_step(cpu);
    }
    if(err==0){           //if overflows occurs, test stops but desk register is still writen.
        err = (mips_error)(err | mips_cpu_get_register(cpu, 3, &got) );
    }
    if(err==0)
        passed = (err == mips_Success) && ( got == 0x8877);
    
    mips_test_end_test(testId, passed, NULL);
    
    
    ////////////////////////////////////////////////////////////////////////
    
    //testing LWR
    
    testId=mips_test_begin_test("lwr");             //get the ID
    passed=0;
    
    err = mips_cpu_set_register(cpu, 2, 0x00009C40ul);               //base  = 40000
    if(err==0)
        err = mips_cpu_set_register(cpu, 3, 0x00000000ul);
    
    AND_instr = 0x98430FA1;    // I-type  : 101000 00010 00011 00000 00000 010000, offset = 4001
    invert_bytes(&AND_instr);
    if(err==0)
        err = mips_cpu_get_pc(cpu, &current_addr);
    
    if(err==0)
        err = mips_mem_write(mem,current_addr,4,(uint8_t*)&AND_instr);
    
    data_32 = 0x66778899;
    //    data_32_pre = 0x11223344;
    data_32_post = 0xAABBCCDD;
    invert_bytes(&data_32);
    //    invert_bytes(&data_32_pre);
    invert_bytes(&data_32_post);
    
    
    
    data_mem = 0x00009C40 + 0xFA4; // (must be 4's multiplies)
    if(err==0)
        err = mips_mem_write(mem,data_mem,4,(uint8_t*)&data_32_post);        //0xDDCCBBAA in mem
    
    data_mem = 0x00009C40 + 0xFA0; // (must be 4's multiplies)
    if(err==0)
        err = mips_mem_write(mem,data_mem,4,(uint8_t*)&data_32);        //0x99887766 in mem
    
    //    data_mem = 0x00009C40 + 0xF9C; // (must be 4's multiplies)
    //    if(err==0)
    //        err = mips_mem_write(mem,data_mem,4,(uint8_t*)&data_32_pre);        //0x44332211 in mem
    
    if(err==0){
        cout<<"CPU running lwr "<<endl;
        err=mips_cpu_step(cpu);
    }
    if(err==0){           //if overflows occurs, test stops but desk register is still writen.
        err = (mips_error)(err | mips_cpu_get_register(cpu, 3, &got) );
    }
    if(err==0)
        passed = (err == mips_Success) && ( got == 0x66DD);
    
    mips_test_end_test(testId, passed, NULL);
    
    
    ////////////////////////////////////////////////////////////////////////
    
    
    
    
    ////////////////////////////////////////////////////////////////////
    //testing J
    
    testId=mips_test_begin_test("j");             //get the ID
    passed=0;
    
    
    AND_instr = 0x08000001;    // J_type  0000 10ii iiii iiii iiii iiii iiii iiii
    invert_bytes(&AND_instr);
    if(err==0)
        err = mips_cpu_get_pc(cpu, &current_addr);
    
    if(err==0)
        err = mips_mem_write(mem,current_addr,4,(uint8_t*)&AND_instr);
    
    if(err==0)
        err = mips_cpu_get_pcN(cpu, &nPC);

    if(err==0){
        cout<<"CPU running j"<<endl;
        err=mips_cpu_step(cpu);
    }
    
    if(err==0)
        err = mips_cpu_get_pcN(cpu, &test_addr);    //nPC after step into CPU
    

    passed = (err == mips_Success) && (test_addr == ((nPC & 0xF0000000) + 4));
    
    
    mips_test_end_test(testId, passed, NULL);
    
    
    ////////////////////////////////////////////////////////////////////
    //testing JAL
    
    testId=mips_test_begin_test("jal");             //get the ID
    passed=0;
    
    
    AND_instr = 0x0C000001;    // J_type  0000 10ii iiii iiii iiii iiii iiii iiii
    invert_bytes(&AND_instr);
    if(err==0)
        err = mips_cpu_get_pc(cpu, &current_addr);
    
    if(err==0)
        err = mips_mem_write(mem,current_addr,4,(uint8_t*)&AND_instr);
    
    if(err==0)
        err = mips_cpu_get_pcN(cpu, &nPC);          //nPC before step into CPU
    
    
    if(err==0){
        cout<<"CPU running jal"<<endl;
        err=mips_cpu_step(cpu);
    }
    
    if(err==0)
        err =  mips_cpu_get_register(cpu, 31, &return_addr);     //store returning address
    
    if(err==0)
        err = mips_cpu_get_pcN(cpu, &test_addr);    //nPC after step into CPU

    
    
    passed = (err == mips_Success) && ((test_addr == ( (nPC & 0xF0000000) + 4)) && (return_addr == (current_addr + 8) ));
    
    mips_test_end_test(testId, passed, NULL);
    
    
    ////////////////////////////////////////////////////////////////////
    //testing JR
    
    testId=mips_test_begin_test("jr");             //get the ID
    passed=0;
    
    
    AND_instr = 0x00400008;    // J_type  0000 10ii iiii iiii iiii iiii iiii iiii
    invert_bytes(&AND_instr);
    if(err==0)
        err = mips_cpu_get_pc(cpu, &current_addr);
    
    if(err==0)
        err = mips_cpu_set_register(cpu, 2, 0x00FFFF00);               //2147483648

    if(err==0)
        err = mips_mem_write(mem,current_addr,4,(uint8_t*)&AND_instr);
    
    if(err==0)
        err = mips_cpu_get_pcN(cpu, &nPC);          //nPC before step into CPU
    
    
    if(err==0){
        cout<<"CPU running jr"<<endl;
        err=mips_cpu_step(cpu);
    }
    
    
    if(err==0)
        err = mips_cpu_get_pcN(cpu, &test_addr);    //nPC after step into CPU
    
    
    
    passed = (err == mips_Success) && (test_addr == 0x00FFFF00  );
    
    mips_test_end_test(testId, passed, NULL);
    
    
    ////////////////////////////////////////////////////////////////////
    
    
    
    
    mips_test_end_suite();
    

    
    mips_cpu_free(cpu);
    mips_mem_free(mem);
    cout<<"test finished, memory is free now"<<endl;
    return 0;
}