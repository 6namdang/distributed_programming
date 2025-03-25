Psuedo Pipeline Stage
**
**Summary:
** This code models 6 stages of a pipeline. Each pipeline stage is broken into 2 sections.
** The first section is the Input which pulls data in from the previous stage's output.
** The second section is the Output which processes data from the Input section then waits
** for the next stage's Input to pull the data.
**
**Functions:
** Function Main declares all variables, regulates clocks, manages input data, calls external functions
** for data manipulation and prints results.
** Function READIN checks if there is new data to be acquired and, if applicable, copies data from previous stage
** Function GETOUT checks if last stage has output ready to exit pipeline
**
**Test Plan:
** Input file pushes in following values:
** 00000000000000000000000000000001
** 00000000000000000000000000000011
** 00000000000000000000000000000111
** 00000000000000000000000000001111
** 00000000000000000000000000011111
** 00000000000000000000000000111111
** 00000000000000000000000001111111
** 00000000000000000000000011111111
**
************/
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;
// prototype functions
void READIN(bool &Sprev_readyout, int Sprev_processed, bool &Sn_readyin, int &Sn_data, bool first_term);
void WRITEOUT(bool &Sn_readyin, int &Sn_data, int &Sn_processed, bool &Sn_readyout, bool Snext_readyin);
void GETOUT(bool &Sn_readyout, int Sn_processed, int output);
// Main program declares all variables, regulates clock, manages input data, calls external functions for data manipulation, and prints results
to console void main()
{
    // Declare Variables
    bool clk = 0;                                                                                                                                   // operations are executed on the rising and falling edges of the clock signal
    char count = 0;                                                                                                                                 // used to control number of loops after all input data has been expended
    char PC = 0;                                                                                                                                    // pipeline count
    unsigned char clk_counter = 0;                                                                                                                  // counts number of clock tics (ie. each edge counts as one tic)
    int newdata = NULL, output = NULL;                                                                                                              // integer value entering and exiting the pipeline, respectively
    bool c1 = 0;                                                                                                                                    // constant bool false
    bool c2 = 1;                                                                                                                                    // constant bool true
    int c3 = 0;                                                                                                                                     // constant int 0
    bool s0_readyin = 1, s1_readyin = 1, s2_readyin = 1, s3_readyin = 1, s4_readyin = 1, s5_readyin = 1;                                            // Stage input flag, when
    high, stage is ready to receive data bool s0_readyout = 0, s1_readyout = 0, s2_readyout = 0, s3_readyout = 0, s4_readyout = 0, s5_readyout = 0; // Stage output flag, when
    high, stage is ready to output data int s0_data = NULL, s1_data = NULL, s2_data = NULL, s3_data = NULL, s4_data = NULL, s5_data = NULL;         // Stage data
    buffer, holds unprocessed integer value int s0_processed = NULL, s1_processed = NULL, s2_processed = NULL, s3_processed = NULL, s4_processed = NULL,
                                                s5_processed = NULL; // Stage output buffer, holds processed integer value
    // Open input file ifstream in_file;
    in_file.open("initial_data.txt");
    // Verify file integrity if (in_file == NULL)
    {
        printf("No input file! \n");
    }
    // Loop until all data has passed through pipeline
    while (count < 6) // number of iterations after last line of data is read (equal to number of pipeline stages)
    {
        // Get input data if (!in_file.eof())
        {
            in_file >> newdata; // new data from input file
        }
        else // End of input file
        {
            newdata = NULL; // no new data count++; //increment termination count
        }
        // Rising Edge - Pass data from Stage to Stage (including input and output) if (clk == 0)
        {
            clk = 1; // toggle clock clk_counter++; //increment counter
            PC++;    // increment pipeline count
                     // Check last stage of pipeline for output data GETOUT(s5_readyout, s5_processed, output);
                     // Check previous stage of pipeline for new data READIN(s4_readyout, s4_processed, s5_readyin, s5_data, false); READIN(s3_readyout,
s3_processed, s4_readyin, s4_data, false);
READIN(s2_readyout, s2_processed, s3_readyin, s3_data, false);
READIN(s1_readyout, s1_processed,
       s2_readyin, s2_data, false);
READIN(s0_readyout, s0_processed, s1_readyin, s1_data, false);
READIN(c2, newdata, s0_readyin, s0_data, true);
// Print Results
printf("Clock count: %d", clk_counter);
printf(" Pipeline Count: %d\n", PC);
printf("Stage 5 (in): %d (out): %d instruction:
%032d\n",!s5_readyin,s5_readyout,s5_data);
printf("Stage 4 (in): %d (out): %d instruction:
%032d\n",!s4_readyin,s4_readyout,s4_data);
printf("Stage 3 (in): %d (out): %d instruction:
%032d\n",!s3_readyin,s3_readyout,s3_data);
printf("Stage 2 (in): %d (out): %d instruction:
%032d\n",!s2_readyin,s2_readyout,s2_data);
printf("Stage 1 (in): %d (out): %d instruction:
%032d\n",!s1_readyin,s1_readyout,s1_data);
printf("Stage 0 (in): %d (out): %d instruction:
%032d\n",!s0_readyin,s0_readyout,s0_data);
printf("\n");
        } // end of rising edge procedure
        // Falling Edge - Process data within each stage if (clk == 1)
        {
            clk = 0; // toggle clock
            // Process data within each stage
            WRITEOUT(s5_readyin, s5_data, s5_processed, s5_readyout, c1);
            WRITEOUT(s4_readyin, s4_data, s4_processed, s4_readyout, s5_readyin);
            WRITEOUT(s3_readyin, s3_data, s3_processed, s3_readyout, s4_readyin);
            WRITEOUT(s2_readyin, s2_data, s2_processed, s2_readyout, s3_readyin);
            WRITEOUT(s1_readyin, s1_data, s1_processed, s1_readyout, s2_readyin);
            WRITEOUT(s0_readyin, s0_data, s0_processed, s0_readyout, s1_readyin);
            // Print Results
            printf("Clock count: %d", clk_counter);
            printf(" Pipeline Count: %d\n", PC);
printf("Stage 5 (in): %d (out): %d instruction:
%032d\n",!s5_readyin,s5_readyout,s5_processed);
printf("Stage 4 (in): %d (out): %d instruction:
%032d\n",!s4_readyin,s4_readyout,s4_processed);
printf("Stage 3 (in): %d (out): %d instruction:
%032d\n",!s3_readyin,s3_readyout,s3_processed);
printf("Stage 2 (in): %d (out): %d instruction:
%032d\n",!s2_readyin,s2_readyout,s2_processed);
printf("Stage 1 (in): %d (out): %d instruction:
%032d\n",!s1_readyin,s1_readyout,s1_processed);
printf("Stage 0 (in): %d (out): %d instruction:
%032d\n",!s0_readyin,s0_readyout,s0_processed);
printf("\n");
        } // end falling edge
    } // end while
    cin >> output; // program parks here
} // end of main
/*/ ------------------- Manipulating Functions --------------------------------------------
---/*/
/*/
*Function READIN checks if there is new data to be acquired and, if applicable, copies data from previous stage
* Inputs: &PrevStageReadyoutFlag, PrevStageProcessedData, &StageReadyinFlag, &StageUnprocessedData, first term identifier
* Outputs: This function has no outputs but may change referenced variable values if required
/*/
void READIN(bool &Sprev_readyout, int Sprev_processed, bool &Sn_readyin, int &Sn_data, bool first_term)
{
    if (Sn_readyin == 1) // verify no data in input of Sn
    {
        if (Sprev_readyout == 1) // verify data in ouput of Sprev
        {
            Sn_data = Sprev_processed; // copy data from Sprev to Sn Sn_readyin = 0;//reset input ready flag for Sn
            if (!first_term)
            {
                Sprev_readyout = 0; // tell previous stage data has been taken
            }
        }
    }
}
/*/--------------------------------------------------------------------------------------
*Function WRITEOUT checks if next stage has taken current stage's data and checks if current stage has new unprocessed data.
* If so, it processes the data, clears old data, and sets input and output ready flags.
* Inputs: &StageReadyinFlag, &StageUnprocessedData, &StageProcessedData, &StageReadyoutFlag, NextStageReadyinFlag
* Outputs: This function has no outputs but may change referenced variable values if required
/*/
void WRITEOUT(bool &Sn_readyin, int &Sn_data, int &Sn_processed, bool &Sn_readyout, bool Snext_readyin)
{
    // Process data if Sn contains new data. Check if Snext has taken processed data. if (Sn_readyout == 0)//output empty
    {
        if (Sn_readyin == 0) // Sn input has data
        {
            data is processed **
        }
        Sn_processed = Sn_data; // move data from input to output **This is where
        Sn_data = NULL;         // clear input data Sn_readyout = 1;//Set ouput ready flag Sn_readyin = 1;//Set input ready flag
    }
    else // Sn output contains data
    {
        if (Snext_readyin == 0) // Snext is ready for input
        {
            Sn_readyout = 0; // reset output ready flag
        }
    }
}
/*/------------------------------------------------------------------------------
*Function GETOUT checks if last stage has output ready to exit pipeline
* Inputs: &StageReadyoutFlag, StageProcessedData, Output
* Outputs: This function has no outputs but may change referenced variable values if required
/*/
void GETOUT(bool &Sn_readyout, int Sn_processed, int output)
{
    if (Sn_readyout == 1) // verify data in ouput of Sprev
    {
        output = Sn_processed; // copy data from Sprev to Sn Sn_readyout = 0;//reset input ready flag for Sn
    }
}