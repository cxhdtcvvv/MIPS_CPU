//
//  mips_cpu.cpp
//  MIPS_simulator
//
//  Created by Xiaohan Chen on 21/10/2014.
//  Copyright (c) 2014 Xiaohan Chen. All rights reserved.
//



/* This file is an implementation of the functions
 defined in mips_cpu.h. It is designed to be
 linked against something which needs an implementation
 of a processor
 */


#include "mips.h"
#include "mips_cpu.h"

#include <stdlib.h>
#include <iostream>

using namespace std;


uint32_t extractbits(int num_bits, int No_firt_bit, uint32_t instruction){      //extract bits from 32bits data
    instruction = instruction >> No_firt_bit;
    instruction = instruction & ( (0x1 << num_bits) -1);
    
    return instruction;
}



void invert_bytes(uint32_t*instruction){
    uint32_t temp = 0;
    temp = (*instruction >>24) & ( (0x1 << 8) -1);      // [31:24] -> [7:0] ,,,and with 11111111;
    temp = temp |( (*instruction >>8) & (( (0x1 << 8) -1) << 8));       //3rd ->2nd
    temp = temp |( (*instruction <<8) & (( (0x1 << 8) -1) << 16));       //2nd ->3rd
    temp = temp |( (*instruction <<24) & (( (0x1 << 8) -1) << 24));       //1st ->4th
    
    *instruction = temp;
}




struct mips_cpu_impl{
    
    uint32_t pc;
    uint32_t pcN;
    uint32_t regs[32];
    uint32_t LO, HI;
    
    mips_mem_h mem;
};


void get_LO(mips_cpu_h state, uint32_t *reg_LO){
    *reg_LO = state->LO;
}

void get_HI(mips_cpu_h state, uint32_t *reg_HI){
    *reg_HI = state->HI;
}



mips_cpu_h mips_cpu_create(mips_mem_h mem)
{
    unsigned i;
    mips_cpu_h res=(mips_cpu_h)malloc(sizeof(struct mips_cpu_impl));
    
    res->mem=mem;
    res->LO = 0;
    res->HI = 0;

    res->pc=0;
    res->pcN=4;
    
    for( i=0;i<32;i++){                 //reset reg
        res->regs[i]=0;
    }
    
    return res;
}

void mips_cpu_free(mips_cpu_h state)
{
    if(state){
        state->pc =0;
        state->pcN =0;
        state->HI = 0;
        state->LO = 0;
        for(int i=0;i<32;i++){                 //reset reg
            state->regs[i]=0;
        }
        free(state);
        
    }
}

mips_error mips_cpu_get_register(
                                 mips_cpu_h state,	//!< Valid (non-empty) handle to a CPU
                                 unsigned index,		//!< Index from 0 to 31
                                 uint32_t *value		//!< Where to write the value to, ////pass by point??????
)
{
    if(state==0)
        return mips_ErrorInvalidHandle;
    if(index>=32)
        return mips_ErrorInvalidArgument;
    if(value==0)
        return mips_ErrorInvalidArgument;
    
    *value = state->regs[index];
    return mips_Success;
}

/*! Modifies one of the 32 general purpose MIPS registers. */
mips_error mips_cpu_set_register(
                                 mips_cpu_h state,	//!< Valid (non-empty) handle to a CPU
                                 unsigned index,		//!< Index from 0 to 31
                                 uint32_t value		//!< New value to write into register file
)
{
    if(state==0)
        return mips_ErrorInvalidHandle;
    if(index>=32)
        return mips_ErrorInvalidArgument;
    
    // TODO : What about register zero?
    state->regs[index]=value;
    
    return mips_Success;
}

mips_error mips_cpu_step(mips_cpu_h state)
{
    uint32_t PC;
    mips_error err = mips_cpu_get_pc(state, &PC);               //get the current value of PC
    
    if(state==0)
        return mips_ErrorInvalidHandle;
    
    uint32_t instruction_in;            ///////////bit length?>>?????
    uint32_t rs=0, rt=0, rd=0, h=0, imm=0, mem_addr=0, base = 0;
    int32_t rs_signed=0, rt_signed=0, rd_signed=0, h_signed=0, imm_signed=0;
    
    err = mips_mem_read(state->mem, PC, 4, (uint8_t*) &instruction_in);
    
    invert_bytes(&instruction_in);

    
    if(err == 0){
        if (extractbits(6, 26, instruction_in) ==  0x00) {      //Opcode = R_type
            
            if(extractbits(6, 0, instruction_in) == 0x24){      //Function code = AND
                err =  mips_cpu_get_register(state, extractbits(5, 21, instruction_in), &rs);
                if(err == 0)
                    err =  mips_cpu_get_register(state, extractbits(5, 16, instruction_in), &rt);
                if(err == 0){
                    rd = rs & rt;
                    err = mips_cpu_set_register(state, extractbits(5, 11, instruction_in), rd);
                    if(err == 0){
                        state->pc = state->pcN;
                        state->pcN += 4;
                    }
                } // do math
            }
            
            //////////////////////////////////////////////////////////////////////////////////////

            
            if(extractbits(6, 0, instruction_in) == 0x25){      //Function code = OR
                err =  mips_cpu_get_register(state, extractbits(5, 21, instruction_in), &rs);
                if(err == 0)
                    err =  mips_cpu_get_register(state, extractbits(5, 16, instruction_in), &rt);
                if(err == 0){
                    rd = rs | rt;
                    err = mips_cpu_set_register(state, extractbits(5, 11, instruction_in), rd);
                    if(err == 0){
                        state->pc = state->pcN;
                        state->pcN += 4;
                    }
                } // do math
            }
            
            //////////////////////////////////////////////////////////////////////////////////////

            
            if(extractbits(6, 0, instruction_in) == 0x26){      //Function code = XOR
                err =  mips_cpu_get_register(state, extractbits(5, 21, instruction_in), &rs);
                if(err == 0)
                    err =  mips_cpu_get_register(state, extractbits(5, 16, instruction_in), &rt);
                if(err == 0){
                    rd = rs ^ rt;
                    err = mips_cpu_set_register(state, extractbits(5, 11, instruction_in), rd);
                    if(err == 0){
                        state->pc = state->pcN;
                        state->pcN += 4;
                    }
                } // do math
            }
            
            //////////////////////////////////////////////////////////////////////////////////////
            
            if(extractbits(6, 0, instruction_in) == 0x2B){      //Function code = SLTU, if s < t , d = 1; otherwise d = 0, unsigned;
                err =  mips_cpu_get_register(state, extractbits(5, 21, instruction_in), &rs);
                if(err == 0)
                    err =  mips_cpu_get_register(state, extractbits(5, 16, instruction_in), &rt);
                if(err == 0){
                    if(rs< rt)
                        rd = 1;
                    else
                        rd = 0;
                    err = mips_cpu_set_register(state, extractbits(5, 11, instruction_in), rd);
                    if(err == 0){
                        state->pc = state->pcN;
                        state->pcN += 4;
                    }
                } // do math
            }
            
            //////////////////////////////////////////////////////////////////////////////////////
            
            if(extractbits(6, 0, instruction_in) == 0x2A){      //Function code = SLT, if rs < rt , rd = 1; otherwise d = 0, unsigned;
                err =  mips_cpu_get_register(state, extractbits(5, 21, instruction_in), &rs);
                if(err == 0)
                    err =  mips_cpu_get_register(state, extractbits(5, 16, instruction_in), &rt);
                if(err == 0){
                    rs_signed = rs;
                    rt_signed = rt;
                    if(rs_signed < rt_signed)
                        rd = 1;
                    else
                        rd = 0;
                    err = mips_cpu_set_register(state, extractbits(5, 11, instruction_in), rd);
                    if(err == 0){
                        state->pc = state->pcN;
                        state->pcN += 4;
                    }
                } // do math
            }
            
            //////////////////////////////////////////////////////////////////////////////////////
            
            if(extractbits(6, 0, instruction_in) == 0x21){      //Function code = ADDU
                err =  mips_cpu_get_register(state, extractbits(5, 21, instruction_in), &rs);
                if(err == 0)
                    err =  mips_cpu_get_register(state, extractbits(5, 16, instruction_in), &rt);
                if(err == 0){
                    rd = rs + rt;
                    err = mips_cpu_set_register(state, extractbits(5, 11, instruction_in), (uint32_t)rd);
                    if(err == 0){
                        state->pc = state->pcN;
                        state->pcN += 4;
                    }
                } // do math
            }
            
            //////////////////////////////////////////////////////////////////////////////////////
            
            if(extractbits(6, 0, instruction_in) == 0x23){      //Function code = SUBU
                err =  mips_cpu_get_register(state, extractbits(5, 21, instruction_in), &rs);
                if(err == 0)
                    err =  mips_cpu_get_register(state, extractbits(5, 16, instruction_in), &rt);
                if(err == 0){
                    rd = rs - rt;
                    err = mips_cpu_set_register(state, extractbits(5, 11, instruction_in), (uint32_t)rd);
                    if(err == 0){
                        state->pc = state->pcN;
                        state->pcN += 4;
                    }
                } // do math
            }
            
            //////////////////////////////////////////////////////////////////////////////////////
            
            if(extractbits(6, 0, instruction_in) == 0x20){      //Function code = ADD, with overflow
                err =  mips_cpu_get_register(state, extractbits(5, 21, instruction_in), &rs);
                if(err == 0)
                    err =  mips_cpu_get_register(state, extractbits(5, 16, instruction_in), &rt);
                if(err == 0){
                    
                    rs_signed = rs;
                    rt_signed = rt;
                    rd_signed = rs_signed + rt_signed;                   //0x80000000
                
                    err = mips_cpu_set_register(state, extractbits(5, 11, instruction_in), (uint32_t)rd_signed);
                    
                    if(err == 0){
                        state->pc = state->pcN;
                        state->pcN += 4;
                    }
                }
                
                if((rs_signed > 0 && rt_signed > 0) && rd_signed < 0){
                    cout<<"overflow happens in ADD"<<endl;
                    return mips_ExceptionArithmeticOverflow;
                }
                
                else if((rs_signed < 0 && rt_signed < 0) && rd_signed > 0){
                    cout<<"overflow"<<endl;
                    return mips_ExceptionArithmeticOverflow;
                }
                
                else
                    return mips_Success;
            }
            
            
            //////////////////////////////////////////////////////////////////////////////////////
            
            if(extractbits(6, 0, instruction_in) == 0x22){      //Function code = SUB, with overflow
                err =  mips_cpu_get_register(state, extractbits(5, 21, instruction_in), &rs);
                if(err == 0)
                    err =  mips_cpu_get_register(state, extractbits(5, 16, instruction_in), &rt);
                if(err == 0){
                    
                    rs_signed = rs;
                    rt_signed = rt;
                    rd_signed = rs_signed - rt_signed;                   //0x80000000
                    
                    if((rs_signed > 0 && -rt_signed > 0) && rd_signed < 0){
                        cout<<"overflow happens in ADD"<<endl;
                        return mips_ExceptionArithmeticOverflow;
                    }
                    
                    else if((rs_signed < 0 && -rt_signed < 0) && rd_signed > 0){
                        cout<<"overflow"<<endl;
                        return mips_ExceptionArithmeticOverflow;
                    }
                    
                    else
                        err = mips_cpu_set_register(state, extractbits(5, 11, instruction_in), (uint32_t)rd_signed);
                    
                    
                    
                    if(err == 0){
                        state->pc = state->pcN;
                        state->pcN += 4;
                    }
                }
            }
            
            
            
            //////////////////////////////////////////////////////////////////////////////////////
            
            if(extractbits(6, 0, instruction_in) == 0x03){      //Function code = SRA, with overflow, The sign bit is shifted in.
                h = extractbits(5, 6, instruction_in);    //extract value for offset
                if(err == 0)
                    err =  mips_cpu_get_register(state, extractbits(5, 16, instruction_in), &rt);
                if(err == 0){
                    
                    rt_signed = rt;
                    rd = rt_signed >>h;
                    
                    err = mips_cpu_set_register(state, extractbits(5, 11, instruction_in), (uint32_t)rd);
                    
                    if(err == 0){
                        state->pc = state->pcN;
                        state->pcN += 4;
                    }
                }
            }
            
            
            
            //////////////////////////////////////////////////////////////////////////////////////
  
            if(extractbits(6, 0, instruction_in) == 0x02){      //Function code = SRL, with overflow, 0 is shifted in
                h = extractbits(5, 6, instruction_in);    //extract value for offset
                if(err == 0)
                    err =  mips_cpu_get_register(state, extractbits(5, 16, instruction_in), &rt);
                if(err == 0){
                    
                    rd = rt >> h;                   //right shift？？？？？what if h > 32???????????????????????????***********
                    
                    err = mips_cpu_set_register(state, extractbits(5, 11, instruction_in), (uint32_t)rd);
                    
                    if(err == 0){
                        state->pc = state->pcN;
                        state->pcN += 4;
                    }
                }
            }
            
            //////////////////////////////////////////////////////////////////////////////////////
            
            if(extractbits(6, 0, instruction_in) == 0x00){      //Function code = SLL, with overflow, 0 is shifted in
                h = extractbits(5, 6, instruction_in);    //extract value for offset
                if(err == 0)
                    err =  mips_cpu_get_register(state, extractbits(5, 16, instruction_in), &rt);
                if(err == 0){
                    
                    rd = rt << h;                   //right shift？？？？？what if h > 32???????????????????????????***********
                    
                    err = mips_cpu_set_register(state, extractbits(5, 11, instruction_in), (uint32_t)rd);
                    
                    if(err == 0){
                        state->pc = state->pcN;
                        state->pcN += 4;
                    }
                }
            }
            
            //////////////////////////////////////////////////////////////////////////////////////

            
            if(extractbits(6, 0, instruction_in) == 0x06){      //Function code = SRLV, with overflow, 0 is shifted in
                err =  mips_cpu_get_register(state, extractbits(5, 21, instruction_in), &rs);
                if(err == 0)
                    err =  mips_cpu_get_register(state, extractbits(5, 16, instruction_in), &rt);
                if(err == 0){
                    
                    rd = rt >> rs;
                    
                    err = mips_cpu_set_register(state, extractbits(5, 11, instruction_in), (uint32_t)rd);
                    
                    if(err == 0){
                        state->pc = state->pcN;
                        state->pcN += 4;
                    }
                }
            }
            
            //////////////////////////////////////////////////////////////////////////////////////
            
            
            
            if(extractbits(6, 0, instruction_in) == 0x04){      //Function code = SLLV, with overflow, 0 is shifted in
                err =  mips_cpu_get_register(state, extractbits(5, 21, instruction_in), &rs);
                if(err == 0)
                    err =  mips_cpu_get_register(state, extractbits(5, 16, instruction_in), &rt);
                if(err == 0){
                    
                    rd = rt << rs;
                    
                    err = mips_cpu_set_register(state, extractbits(5, 11, instruction_in), (uint32_t)rd);
                    
                    if(err == 0){
                        state->pc = state->pcN;
                        state->pcN += 4;
                    }
                }
            }
            
            //////////////////////////////////////////////////////////////////////////////////////

            if(extractbits(6, 0, instruction_in) == 0x06){      //Function code = ANDL, with overflow, 0 is shifted in
                err =  mips_cpu_get_register(state, extractbits(5, 21, instruction_in), &rs);
                if(err == 0)
                    err =  mips_cpu_get_register(state, extractbits(5, 16, instruction_in), &rt);
                if(err == 0){
                    
                    rd = rt >> rs;
                    
                    err = mips_cpu_set_register(state, extractbits(5, 11, instruction_in), (uint32_t)rd);
                    
                    if(err == 0){
                        state->pc = state->pcN;
                        state->pcN += 4;
                    }
                }
            }
            
            
        
        ////////////////////////////////////////////////////////////////////////////////////////
        
        if(extractbits(6, 0, instruction_in) == 0x1A){      //Function code = DIV, 11010
            
            err =  mips_cpu_get_register(state, extractbits(5, 21, instruction_in), &rs);
            if(err == 0)
                err =  mips_cpu_get_register(state, extractbits(5, 16, instruction_in), &rt);
            if(err == 0){
                int32_t remainder= 0, quotient = 0;
                
                rs_signed = rs;
                rt_signed = rt;
                
                
                quotient = rs_signed / rt_signed;
                
                remainder = rs_signed % rt_signed;
                
//                cout<<quotient<<"    quotient"<<endl;
//                cout<<remainder<<"    remainder"<<endl;

                
                state->LO = quotient;
                state->HI = remainder;
                
                
                if(err == 0){
                    state->pc = state->pcN;
                    state->pcN += 4;
                }
            }
        }
        
        ////////////////////////////////////////////////////////////////////////////////////////
        if(extractbits(6, 0, instruction_in) == 0x1B){      //Function code = DIVU, 11011
            
            err =  mips_cpu_get_register(state, extractbits(5, 21, instruction_in), &rs);
            if(err == 0)
                err =  mips_cpu_get_register(state, extractbits(5, 16, instruction_in), &rt);
            if(err == 0){
                int32_t remainder= 0, quotient = 0;

                
                quotient = rs / rt;
                remainder = rs % rt;
                
                state->LO = quotient;
                state->HI = remainder;
                
                
                if(err == 0){
                    state->pc = state->pcN;
                    state->pcN += 4;
                }
            }
        }
        
        ////////////////////////////////////////////////////////////////////////////////////////
            
            if(extractbits(6, 0, instruction_in) == 0x18){      //Function code = MULT, 011000; result is 64bits, HI & LO
                
                err =  mips_cpu_get_register(state, extractbits(5, 21, instruction_in), &rs);
                if(err == 0)
                    err =  mips_cpu_get_register(state, extractbits(5, 16, instruction_in), &rt);
                if(err == 0){
                    int64_t result_mult;
                    int64_t temp_rs  , temp_rt;
                    uint64_t temp;
                    
                    if(rs>>31 == 1)
                        temp_rs = rs | 0xFFFFFFFF00000000;
                    else
                        temp_rs =rs;
                    
                    if(rt>>31 == 1)
                        temp_rt = rt | 0xFFFFFFFF00000000;
                    else
                        temp_rt =rt;
                    
                    result_mult = temp_rs * temp_rt;
                    temp = result_mult;
                                        
                    state->LO = ((temp<<32)>>32);                   //confirmed working
                    state->HI = (temp >> 32) | 0x00000000;
                    
                    state->pc = state->pcN;
                    state->pcN += 4;
                    
                }
            }
            
            ////////////////////////////////////////////////////////////////////////////////////////
            
            
            if(extractbits(6, 0, instruction_in) == 0x19){      //Function code = MULTU, 011000; result is 64bits, HI & LO
                
                err =  mips_cpu_get_register(state, extractbits(5, 21, instruction_in), &rs);
                if(err == 0)
                    err =  mips_cpu_get_register(state, extractbits(5, 16, instruction_in), &rt);
                if(err == 0){
                    uint64_t result_mult;
                    uint64_t temp_rs = rs , temp_rt = rt;

                    
                    result_mult = temp_rs * temp_rt;
                    
                    state->LO = ((result_mult<<32)>>32);                   //confirmed working
                    state->HI = (result_mult >> 32) | 0x00000000;
                    
                    state->pc = state->pcN;
                    state->pcN += 4;
                    
                }
            }
            
            ////////////////////////////////////////////////////////////////////////////////////////

            if(extractbits(6, 0, instruction_in) == 0x10){      //Function code = MFHI, 010000; result is 64bits, HI & LO
                
                
                err =  mips_cpu_set_register(state, extractbits(5, 11, instruction_in), state->HI);
                
                if(err == 0){
                    state->pc = state->pcN;
                    state->pcN += 4;
                }
                
            }
            
            ////////////////////////////////////////////////////////////////////////////////////////
            
            if(extractbits(6, 0, instruction_in) == 0x12){      //Function code = MFLO, 010010; result is 64bits, HI & LO
                
                
                err =  mips_cpu_set_register(state, extractbits(5, 11, instruction_in), state->LO);
                
                if(err == 0){
                    state->pc = state->pcN;
                    state->pcN += 4;
                }
                
            }
            
            ////////////////////////////////////////////////////////////////////////////////////////
            
            if ( (extractbits(6, 0, instruction_in) ==  0x8)) {      //JR (R type) 001000(26)
                
                err =  mips_cpu_get_register(state, extractbits(5, 21, instruction_in), &rs);

                state->pc = state->pcN;
                state->pcN = rs;
            }
            
            ////////////////////////////////////////////////////////////////////////////////////////
            
            
            
            


    }

    //***********end of R-type
        ////////////////////////////////////////////////////////////////////////////////////////***************I-type

        
        if (extractbits(6, 26, instruction_in) ==  0xC) {      //ANDI, 0011 00(26), ss sss(21), t tttt(16), iiii iiii iiii iiii
            

            imm = extractbits(16, 0, instruction_in);  //ZERO extended
            err =  mips_cpu_get_register(state, extractbits(5, 16, instruction_in), &rt);   //reg2
            if(err == 0){
                
                rs = rt & imm;                   //right shift？？？？？what if h > 32???????????????????????????***********
                
                err = mips_cpu_set_register(state, extractbits(5, 21, instruction_in), (uint32_t)rs);  //reg1
                
                if(err == 0){
                    state->pc = state->pcN;
                    state->pcN += 4;
                }
            }
        }
        
        
        ////////////////////////////////////////////////////////////////////////////////////////
        
        if (extractbits(6, 26, instruction_in) ==  0xD) {      //ORI, 0011 00(26), ss sss(21), t tttt(16), iiii iiii iiii iiii
            
            
            imm =  extractbits(16, 0, instruction_in);  //zero enxtended imm
            err =  mips_cpu_get_register(state, extractbits(5, 16, instruction_in), &rt);
            if(err == 0){
                
                rs = rt | imm;
                
                err = mips_cpu_set_register(state, extractbits(5, 21, instruction_in), (uint32_t)rs);
                
                if(err == 0){
                    state->pc = state->pcN;
                    state->pcN += 4;
                }
            }
        }
        
        ////////////////////////////////////////////////////////////////////////////////////////
        
        
        
        if (extractbits(6, 26, instruction_in) ==  0xE) {      //XORI, 0011 00(26), ss sss(21), t tttt(16), iiii iiii iiii iiii
            
            
            imm =  extractbits(16, 0, instruction_in);  //zero enxtended imm
            if(err == 0)
                err =  mips_cpu_get_register(state, extractbits(5, 16, instruction_in), &rt);
            if(err == 0){
                
                rs = rt ^ imm;
                err = mips_cpu_set_register(state, extractbits(5, 21, instruction_in), (uint32_t)rs);
                
                if(err == 0){
                    state->pc = state->pcN;
                    state->pcN += 4;
                }
            }
        }
        
        
        
        ////////////////////////////////////////////////////////////////////////////////////////
        
        
        if (extractbits(6, 26, instruction_in) ==  0x8) {      //ADDI, 0011 00(26), ss sss(21), t tttt(16), iiii iiii iiii iiii
            
            
            imm =  extractbits(16, 0, instruction_in);  //zero enxtended imm
            err =  mips_cpu_get_register(state, extractbits(5, 16, instruction_in), &rt);
            if(err == 0){
                rt_signed = rt;
                rs_signed = rs;
                
                if(imm>>15 == 1)
                    imm_signed = imm | 0xFFFF0000;
                else
                    imm_signed = imm;
                
                rs_signed = rt_signed + imm_signed;                   //right shift？？？？？what if h > 32???????????????????????????***********
                
                if((imm_signed > 0 && rt_signed > 0) && rs_signed < 0){
                    cout<<"overflow happens in ADDI"<<endl;
                    return err = mips_ExceptionArithmeticOverflow;
                }
                
                else if((imm_signed < 0 && rt_signed < 0) && rs_signed > 0){
                    cout<<"overflow happens in ADDI"<<endl;
                    return mips_ExceptionArithmeticOverflow;
                }
                
                else
                    err = mips_cpu_set_register(state, extractbits(5, 21, instruction_in), (uint32_t)rs_signed);
                
            }
            state->pc = state->pcN;
            state->pcN += 4;

        }
        
        
        ////////////////////////////////////////////////////////////////////////////////////////
        
        if (extractbits(6, 26, instruction_in) ==  0x9) {      //ADDIU,usigned, 0011 00(26), ss sss(21), t tttt(16), iiii iiii iiii iiii
            
            
            imm =  extractbits(16, 0, instruction_in);  //zero enxtended imm
            err =  mips_cpu_get_register(state, extractbits(5, 16, instruction_in), &rt);
            if(err == 0){
                if(imm>>15 == 1)
                    imm_signed = imm | 0xFFFF0000;
                else
                    imm_signed = imm;
                
                rs = rt + imm_signed;                   //unsigned, dont care overflow
                
                err = mips_cpu_set_register(state, extractbits(5, 21, instruction_in), (uint32_t)rs);


                if(err == 0){
                    state->pc = state->pcN;
                    state->pcN += 4;
                }
            }
        }

        
        ////////////////////////////////////////////////////////////////////////////////////////
        
        if (extractbits(6, 26, instruction_in) ==  0x2B) {      //SW, STORE WORD,usigned, 0011 00(26), ss sss(21), t tttt(16), iiii iiii iiii iiii
            
            imm =  extractbits(16, 0, instruction_in);  //signed imm
            err =  mips_cpu_get_register(state, extractbits(5, 16, instruction_in), &rt);
            if(err == 0)
                err =  mips_cpu_get_register(state, extractbits(5, 21, instruction_in), &rs);

            
            if(err == 0){
                if(imm>>15 == 1)
                    imm_signed = imm | 0xFFFF0000;
                else
                    imm_signed = imm;
                
                mem_addr = rs + imm_signed;                   //rs + offset

                err = mips_mem_write(state->mem, mem_addr, 4, (uint8_t*)&rt);   //M[rs+imm] <- rt
                
                if(err == 0){
                    state->pc = state->pcN;
                    state->pcN += 4;
                }
            }
        }
        
        
        ////////////////////////////////////////////////////////////////////////////////////////
        
        if (extractbits(6, 26, instruction_in) ==  0x23) {      //LW,LOAD WORD, usigned, 0011 00(26), ss sss(21), t tttt(16), iiii iiii iiii iiii
            
            
            imm =  extractbits(16, 0, instruction_in);  //zero enxtended imm
            
            err =  mips_cpu_get_register(state, extractbits(5, 16, instruction_in), &rt);
            if(err == 0)
                err =  mips_cpu_get_register(state, extractbits(5, 21, instruction_in), &rs);

            
            if(err == 0){
                if(imm>>15 == 1)
                    imm_signed = imm | 0xFFFF0000;
                else
                    imm_signed = imm;
                
                mem_addr = rs + imm_signed;                   //rs + offset
                
                err = mips_mem_read(state->mem, mem_addr, 4, (uint8_t*)&rt);   //M[rs+imm] -> rt
                
                
                if(err == 0){
                    state->pc = state->pcN;
                    state->pcN += 4;
                }
            }
        }

        ////////////////////////////////////////////////////////////////////////////////////////
        
        if (extractbits(6, 26, instruction_in) ==  0xF) {      //LUI ,LOAD UPPER IMM, usigned, 001111(26), ss sss(21), t tttt(16), iiiiiiii iiii iiii
            
            
            imm =  extractbits(16, 0, instruction_in);  //zero enxtended imm

            
            if(err == 0){
                rt = imm << 16; //rs <- imm
                
                
                err = mips_cpu_set_register(state, extractbits(5, 16, instruction_in), (uint32_t)rt);
                
                
                if(err == 0){
                    state->pc = state->pcN;
                    state->pcN += 4;
                }
            }
        }
        
        ////////////////////////////////////////////////////////////////////////////////////////
        
        
        if (extractbits(6, 26, instruction_in) ==  0x4) {      //BEQ(I type) ,LOAD UPPER IMM, usigned, 001111(26), ss sss(21), t tttt(16), iiiiiiii iiii iiii
            
            
            imm =  extractbits(16, 0, instruction_in);  //zero enxtended imm
            if(imm>>15 == 1)
                imm_signed = imm | 0xFFFF0000;
            else
                imm_signed = imm;
            
            err =  mips_cpu_get_register(state, extractbits(5, 16, instruction_in), &rt);
            if(err == 0)
                err =  mips_cpu_get_register(state, extractbits(5, 21, instruction_in), &rs);
            
            if(err == 0){
                if (rt == rs){
                    state->pc = state->pcN;
                    state->pcN += imm_signed<<2;  //18 bits signed
                }
                else{
                    state->pc = state->pcN;
                    state->pcN += 4;
                }
            }
        }
        
        ////////////////////////////////////////////////////////////////////////////////////////
        
        if (extractbits(6, 26, instruction_in) ==  0x5) {      //BNE(I type) ,LOAD UPPER IMM, usigned, 001111(26), ss sss(21), t tttt(16), iiiiiiii iiii iiii
            
            
            imm =  extractbits(16, 0, instruction_in);  //zero enxtended imm
            if(imm>>15 == 1)
                imm_signed = imm | 0xFFFF0000;
            else
                imm_signed = imm;

            
            err =  mips_cpu_get_register(state, extractbits(5, 16, instruction_in), &rt);
            if(err == 0)
                err =  mips_cpu_get_register(state, extractbits(5, 21, instruction_in), &rs);
            
            if(err == 0){
                if (rt != rs){
                    state->pc = state->pcN;
                    state->pcN += imm_signed<<2;  //18 bits signed
                }
                else{
                    state->pc = state->pcN;
                    state->pcN += 4;
                }
            }
        }
        
        ////////////////////////////////////////////////////////////////////////////////////////
        
        if ( (extractbits(6, 26, instruction_in) ==  0x1) && (extractbits(5, 16, instruction_in) == 0x1)) {      //BGEZ (I type) ,LOAD UPPER IMM, usigned, 000001(26), ss sss(21), 00001(16), iiiiiiii iiii iiii
            
            
            imm =  extractbits(16, 0, instruction_in);  //zero enxtended imm
            
            if(imm>>15 == 1)
                imm_signed = imm | 0xFFFF0000;
            else
                imm_signed = imm;

            
            err =  mips_cpu_get_register(state, extractbits(5, 21, instruction_in), &rs);
            
            if(err == 0){
                if ((rs >> 31) == 0){
                    state->pc = state->pcN;
                    state->pcN += (imm_signed<<2);  //18 bits signed
                }
                else{
                    state->pc = state->pcN;
                    state->pcN += 4;
                }
            }
        }
        
        ////////////////////////////////////////////////////////////////////////////////////////
        
        if ( (extractbits(6, 26, instruction_in) ==  0x1) && (extractbits(5, 16, instruction_in) == 0x11)) {      //BGEZAL (I type) ,LOAD UPPER IMM, usigned, 001111(26), ss sss(21), 10001(16), iiiiiiii iiii iiii
            
            
            imm =  extractbits(16, 0, instruction_in);  //zero enxtended imm
            if(imm>>15 == 1)
                imm_signed = imm | 0xFFFF0000;
            else
                imm_signed = imm;

            
            err =  mips_cpu_get_register(state, extractbits(5, 21, instruction_in), &rs);
            
            if(err == 0){
                
                if ((rs >> 31) == 0){
                    err = mips_cpu_set_register(state, 31, (state->pc + 8));          //save the link address into R31

                    state->pc = state->pcN;
                    state->pcN += (imm_signed<<2);  //18 bits signed
                }
                else{
                    state->pc = state->pcN;
                    state->pcN += 4;
                }
            }
        }
        

        
        ////////////////////////////////////////////////////////////////////////////////////////
        
        if ( (extractbits(6, 26, instruction_in) ==  0x7)) {      //BGTZ (I type) ,LOAD UPPER IMM, usigned, 000111(26), ss sss(21), 00000(16), iiiiiiii iiii iiii
            
            
            imm =  extractbits(16, 0, instruction_in);  //zero enxtended imm
            if(imm>>15 == 1)
                imm_signed = imm | 0xFFFF0000;
            else
                imm_signed = imm;

            
            err =  mips_cpu_get_register(state, extractbits(5, 21, instruction_in), &rs);
            rs_signed = rs;
            
            
            if(err == 0){
                if (rs > 0){
                    state->pc = state->pcN + (imm_signed<<2);
                }
                else{
                    state->pc = state->pcN;
                    state->pcN += 4;
                    
                }
            }
        }
        
        ////////////////////////////////////////////////////////////////////////////////////////
        
        if ( (extractbits(6, 26, instruction_in) ==  0x6)) {      //BLEZ (I type) ,LOAD UPPER IMM, usigned, 000110(26), ss sss(21), 00000(16), iiiiiiii iiii iiii
            
            
            imm =  extractbits(16, 0, instruction_in);  //zero enxtended imm
            if(imm>>15 == 1)
                imm_signed = imm | 0xFFFF0000;
            else
                imm_signed = imm;

            
            err =  mips_cpu_get_register(state, extractbits(5, 21, instruction_in), &rs);
            rs_signed = rs;
            
            
            if(err == 0){
                if (rs_signed <= 0){
                    state->pc = state->pcN + (imm_signed<<2);
                }
                else{
                    state->pc = state->pcN;
                    state->pcN += 4;
                }
            }
        }
        
        ////////////////////////////////////////////////////////////////////////////////////////
        
        if ( (extractbits(6, 26, instruction_in) ==  0x1)) {      //BLTZ (I type) ,LOAD UPPER IMM, usigned, 000001(26), ss sss(21), 00000(16), iiiiiiii iiii iiii
            
            
            imm =  extractbits(16, 0, instruction_in);  //zero enxtended imm
            if(imm>>15 == 1)
                imm_signed = imm | 0xFFFF0000;
            else
                imm_signed = imm;

            
            
            err =  mips_cpu_get_register(state, extractbits(5, 21, instruction_in), &rs);
            
            if(err == 0){
                if ((rs >> 31) == 1){
                    state->pc = state->pcN + (imm_signed<<2);
                }
                else{
                    state->pc = state->pcN;
                    state->pcN += 4;
                }
            }
        }
        
        ////////////////////////////////////////////////////////////////////////////////////////
        
        if ( (extractbits(6, 26, instruction_in) ==  0x1) && (extractbits(5, 16, instruction_in) == 0x10)) {      //BLTZAL (I type) ,LOAD UPPER IMM, usigned, 000001(26), ss sss(21), 10001(16), iiiiiiii iiii iiii
            
            
            imm =  extractbits(16, 0, instruction_in);  //zero enxtended imm
            if(imm>>15 == 1)
                imm_signed = imm | 0xFFFF0000;
            else
                imm_signed = imm;

            
            err =  mips_cpu_get_register(state, extractbits(5, 21, instruction_in), &rs);
            
            if(err == 0){
                if ((rs >> 31) == 1){
                    state->pc = state->pcN + (imm_signed<<2);
                    err = mips_cpu_set_register(state, 31, state->pc + 8);          //save the link address into R31
                }
                else{
                    state->pc = state->pcN;
                    state->pcN += 4;
                }
            }
        }
        
        ////////////////////////////////////////////////////////////////////////////////////////
        
        if (extractbits(6, 26, instruction_in) ==  0xA) {      //SLTI, 001010(26), ss sss(21), t tttt(16), iiii iiii iiii iiii
            
            
            imm =  extractbits(16, 0, instruction_in);  //sign enxtended imm
            
            if(imm>>15 == 1)
                imm_signed = imm | 0xFFFF0000;
            else
                imm_signed = imm;
            
            err =  mips_cpu_get_register(state, extractbits(5, 21, instruction_in), &rs);
            if(err == 0){
                rs_signed = rs;

                if (rs_signed < imm_signed) {
                    rt = 1;
                    err = mips_cpu_set_register(state, extractbits(5, 16, instruction_in), (uint32_t)rt);
                }
                else{
                    rt = 0;
                    err = mips_cpu_set_register(state, extractbits(5, 16, instruction_in), (uint32_t)rt);
                }
            }
            
            state->pc = state->pcN;
            state->pcN += 4;
            
            
        }
        
        ////////////////////////////////////////////////////////////////////////////////////////
        
        if (extractbits(6, 26, instruction_in) ==  0xB) {      //SLTIU, 001011(26), ss sss(21), t tttt(16), iiii iiii iiii iiii
            
            //register signed or unsigned????
            imm =  extractbits(16, 0, instruction_in);  //signed enxtended imm
            err =  mips_cpu_get_register(state, extractbits(5, 21, instruction_in), &rs);
            if(err == 0){
                rs_signed = rs;
                
                imm_signed = imm | 0x00000000;      //in order to compare with signed number.

                //cout<<(rs_signed < 1)<<endl;   *cannot compare signed and unsigned*            0x10000000 - 0x00001000 result is overflow...
                if (rs_signed < imm_signed) {
                    rt = 1;
                    err = mips_cpu_set_register(state, extractbits(5, 16, instruction_in), (uint32_t)rt);
                }
                else{
                    rt = 0;
                    err = mips_cpu_set_register(state, extractbits(5, 16, instruction_in), (uint32_t)rt);
                }
                    
                
                if (err == 0) {
                    state->pc = state->pcN;
                    state->pcN += 4;
                }
                
            }
        }
 
        ////////////////////////////////////////////////////////////////////////////////////////
        if (extractbits(6, 26, instruction_in) ==  0x28) {      //SB, 101000(26), ss sss(21), t tttt(16), iiii iiii iiii iiii
            
            
            imm =  extractbits(16, 0, instruction_in);  //sign enxtended imm
            
            if(imm>>15 == 1)
                imm_signed = imm | 0xFFFF0000;
            else
                imm_signed = imm;
            
            err =  mips_cpu_get_register(state, extractbits(5, 21, instruction_in), &base);
            if(err==0){
                err =  mips_cpu_get_register(state, extractbits(5, 16, instruction_in), &rt);
                rt = rt & 0x00FF;       //lSB 8bits
            }
            
            
            if(err == 0){
                uint32_t address, prime_addr, No_byte, previous_data, current_data;
                address =  base + imm_signed;
                
                prime_addr = (address/4) * 4;
                No_byte =  address % 4;
                
                
                err = mips_mem_read(state->mem, prime_addr, 4, (uint8_t*)&previous_data);    //data is already inverted when took out
                
                if(err == 0){
                    uint32_t masking = ( ( (0x1<<8) -1) << ( (3 - No_byte)*8 )  ); //11111111;
                    
                    current_data = ( previous_data & (~masking) ) | ( rt << ( (3 - No_byte)*8 ) );  //replace the specific byte
                    
                    err = mips_mem_write(state->mem, prime_addr, 4, (uint8_t*)&current_data);
                    
                }
            }
            if (err == 0) {
                state->pc = state->pcN;
                state->pcN += 4;
            }
        }
    
    ////////////////////////////////////////////////////////////////////////////////////////
        
        
        if (extractbits(6, 26, instruction_in) ==  0x29) {      //SH , 101001(26), ss sss(21), t tttt(16), iiii iiii iiii iiii
            
            
            imm =  extractbits(16, 0, instruction_in);  //sign enxtended imm
            
            if(imm>>15 == 1)
                imm_signed = imm | 0xFFFF0000;
            else
                imm_signed = imm;
            
            err =  mips_cpu_get_register(state, extractbits(5, 21, instruction_in), &base);
            if(err==0){
                err =  mips_cpu_get_register(state, extractbits(5, 16, instruction_in), &rt);
                rt = rt & 0xFFFF;       //lSB 16bits
            }
            
            
            if(err == 0){
                uint32_t address, prime_addr, No_byte, previous_data, current_data;
                address =  base + imm_signed;
                
                prime_addr = (address/4) * 4;
                No_byte =  address % 4;
                
                
                err = mips_mem_read(state->mem, prime_addr, 4, (uint8_t*)&previous_data);    //data is already inverted when took out
                
                if(err == 0){
                    uint32_t masking = ( ( (0x1<<16) -1) << ( (3 - No_byte)*8 )  ); //11111111;
                    
                    current_data = ( previous_data & (~masking) ) | ( rt << ( (3 - No_byte)*8 ) );  //replace the specific byte
                    
                    err = mips_mem_write(state->mem, prime_addr, 4, (uint8_t*)&current_data);
                    
                }
            }
            if (err == 0) {
                state->pc = state->pcN;
                state->pcN += 4;
            }
        }
        ////////////////////////////////////////////////////////////////////////////////////////
        
        if (extractbits(6, 26, instruction_in) ==  0x20) {      //LB, 100000(26), ss sss(21), t tttt(16), iiii iiii iiii iiii   load as signed value
            
            
            imm =  extractbits(16, 0, instruction_in);  //sign enxtended imm
            
            if(imm>>15 == 1)
                imm_signed = imm | 0xFFFF0000;
            else
                imm_signed = imm;
            
            err =  mips_cpu_get_register(state, extractbits(5, 21, instruction_in), &base);
           
            
            if(err == 0){
                uint32_t address, prime_addr, No_byte, previous_data, current_data;
                address =  base + imm_signed;
                
                prime_addr = (address/4) * 4;
                No_byte =  address % 4;
                
                
                err = mips_mem_read(state->mem, prime_addr, 4, (uint8_t*)&previous_data);    //data is already inverted when took out
                
                uint32_t masking = ( ( (0x1<<8) -1) << ( (3 - No_byte) *8 )  ); //11111111;
                
                current_data = ( previous_data & masking) >>  ( (3 - No_byte)*8 ) ;  //shift back to right
                
                
                if((current_data >> 7) == 1)
                    current_data = current_data | 0xFFFFFF00;           //sign extended
                
                err = mips_cpu_set_register(state, 3, current_data);
                
            }
            if (err == 0) {
                state->pc = state->pcN;
                state->pcN += 4;
            }
        }
        
        ////////////////////////////////////////////////////////////////////////////////////////
        
        if (extractbits(6, 26, instruction_in) ==  0x24) {      //LBU, 100100(26), ss sss(21), t tttt(16), iiii iiii iiii iiii
            
            
            imm =  extractbits(16, 0, instruction_in);  //sign enxtended imm
            
            if(imm>>15 == 1)
                imm_signed = imm | 0xFFFF0000;
            else
                imm_signed = imm;
            
            err =  mips_cpu_get_register(state, extractbits(5, 21, instruction_in), &base);
            
            
            if(err == 0){
                uint32_t address, prime_addr, No_byte, previous_data, current_data;
                address =  base + imm_signed;
                
                prime_addr = (address/4) * 4;
                No_byte =  address % 4;
                
                
                err = mips_mem_read(state->mem, prime_addr, 4, (uint8_t*)&previous_data);    //data is already inverted when took out
                uint32_t masking = ( ( (0x1<<8) -1) << ( (3 - No_byte)*8 )  ); //11111111;
                
                current_data = ( previous_data & masking) >>  ( (3 - No_byte)*8 ) ;  //shift back to right
                
                err = mips_cpu_set_register(state, 3, current_data);
                
            }
            if (err == 0) {
                state->pc = state->pcN;
                state->pcN += 4;
            }
        }
        
        ////////////////////////////////////////////////////////////////////////////////////////
        
        
        
        if ( (extractbits(6, 26, instruction_in) ==  0x2)) {      //J (J type) 000010(26) iiiiiiii iiii iiii
            
            
            imm =  extractbits(26, 0, instruction_in);  //zero enxtended imm
            
            if(err==0){
                imm = (0xF0000000 & state->pcN ) | imm << 2;         //delay slot?????
                state->pc = state->pcN;
                state->pcN = imm;
            }
        }
        
        ////////////////////////////////////////////////////////////////////////////////////////
        if ( (extractbits(6, 26, instruction_in) ==  0x3)) {      //JAL (J type) 000011(26) iiiiiiii iiii iiii
            
            err =  mips_cpu_set_register(state, 31, (state->pc + 8));     //store returning address
            
            if(err == 0)
                imm =  extractbits(26, 0, instruction_in);  //zero enxtended imm
            
            if(err==0){
                imm = (0xF0000000 & state->pcN) | imm << 2;         //delay slot?????
                state->pc = state->pcN;
                state->pcN = imm;
            }
        }
        
        
        ////////////////////////////////////////////////////////////////////////////////////////
        
        if (extractbits(6, 26, instruction_in) ==  0x22) {      //LWL, 100010(26), ss sss(21), t tttt(16), iiii iiii iiii iiii
            
            
            imm =  extractbits(16, 0, instruction_in);  //sign enxtended imm
            
            if(imm>>15 == 1)
                imm_signed = imm | 0xFFFF0000;
            else
                imm_signed = imm;
            
            err =  mips_cpu_get_register(state, extractbits(5, 21, instruction_in), &base);
            
            
            if(err == 0){
                uint32_t temp, address, prime_addr, No_byte, previous_data, previous_data_past, current_data;
                address =  base + imm_signed;              //address to be fetched;
                
                prime_addr = (address/4) * 4;
                No_byte =  address % 4;     //No.0 ~ 3 byte
                
//                err = mips_mem_read(state->mem, prime_addr - 4, 4, (uint8_t*)&previous_data_pre);
                err = mips_mem_read(state->mem, prime_addr, 4, (uint8_t*)&previous_data);    //data is already inverted when took out
                err = mips_mem_read(state->mem, prime_addr + 4, 4, (uint8_t*)&previous_data_past);
                
                uint32_t masking1 =   (0x1<<((4 - No_byte) * 8)) -1 ; //1111 or 11111111 or 111111111111;
//                uint32_t masking2 =   ((0x1<<( No_byte * 4)) -1) << ((4 - No_byte) * 4 )  ; //1111 or 11111111 or 111111111111;

                if(No_byte == 0){
                    err = mips_cpu_set_register(state, extractbits(5, 16, instruction_in), extractbits(16, 16, previous_data));      //directly extract LEFT word
                }
                else{
                    
                    current_data = previous_data & masking1;
                    current_data = current_data << ( No_byte * 8);
                    temp = extractbits(8 * No_byte, ((4 - No_byte ) * 8)  , previous_data_past);
                    current_data = current_data | temp;             //concatenant two words
                    
                    err = mips_cpu_set_register(state, extractbits(5, 16, instruction_in), extractbits(16, 16, current_data));
                    
                }
                
                
            }
            if (err == 0) {
                state->pc = state->pcN;
                state->pcN += 4;
            }
        }
        
        ////////////////////////////////////////////////////////////////////////////////////////
        
        if (extractbits(6, 26, instruction_in) ==  0x26) {      //LWR, 100110(26), ss sss(21), t tttt(16), iiii iiii iiii iiii
            
            imm =  extractbits(16, 0, instruction_in);  //sign enxtended imm
            
            if(imm>>15 == 1)
                imm_signed = imm | 0xFFFF0000;
            else
                imm_signed = imm;
            
            err =  mips_cpu_get_register(state, extractbits(5, 21, instruction_in), &base);
            
            
            if(err == 0){
                uint32_t temp, address, prime_addr, No_byte, previous_data, previous_data_past, current_data;
                address =  base + imm_signed;              //address to be fetched;
                
                prime_addr = (address/4) * 4;
                No_byte =  address % 4;     //No.0 ~ 3 byte
                
//                err = mips_mem_read(state->mem, prime_addr - 4, 4, (uint8_t*)&previous_data_pre);
                err = mips_mem_read(state->mem, prime_addr, 4, (uint8_t*)&previous_data);    //data is already inverted when took out
                err = mips_mem_read(state->mem, prime_addr + 4, 4, (uint8_t*)&previous_data_past);
                
                uint32_t masking1 =   (0x1<<((4 - No_byte) * 8)) -1 ; //1111 or 11111111 or 111111111111;
                //                uint32_t masking2 =   ((0x1<<( No_byte * 4)) -1) << ((4 - No_byte) * 4 )  ; //1111 or 11111111 or 111111111111;
                
                if(No_byte == 0){
                    err = mips_cpu_set_register(state, extractbits(5, 16, instruction_in), extractbits(16, 16, previous_data));      //directly extract LEFT word
                }
                else{
                    
                    current_data = previous_data & masking1;
                    current_data = current_data << ( No_byte * 8);
                    temp = extractbits(8 * No_byte, ((4 - No_byte ) * 8)  , previous_data_past);
                    current_data = current_data | temp;             //concatenant two words
                    
                    err = mips_cpu_set_register(state, extractbits(5, 16, instruction_in), extractbits(16, 0, current_data));
                    
                }
                
                
            }
            if (err == 0) {
                state->pc = state->pcN;
                state->pcN += 4;
            }
        }
        ////////////////////////////////////////////////////////////////////////////////////////
        

    }
    return mips_Success;
}



mips_error mips_cpu_set_pc(mips_cpu_h state, uint32_t pc){      //mips_cpu_h is a pointer type
    
    state->pc = pc;
    return mips_Success;
}


mips_error mips_cpu_get_pc(mips_cpu_h state, uint32_t *pc){             //pc is the pointer pointintg to the address
                                                                        //of the curent instrcution
    
    *pc = state->pc;
    return mips_Success;
}

mips_error mips_cpu_get_pcN(mips_cpu_h state, uint32_t *pcN){
    
    *pcN = state->pcN;
    return mips_Success;
    
}





