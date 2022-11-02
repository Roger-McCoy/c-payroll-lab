// Roger McCoy
// Lab 1 - Special Topics 2347
// A very simple payroll application.
// CITATIONS: 
// WhileRepeatStdC.c by Dr. Burris, SeqIO1.c by Dr. Burris
// C Library - <time.h> tutorialspoint.com/c_standard_library/time_h.htm
// time.h header file in C with Examples. geeksforgeeks.org/time-h-header-file-in-c-with-examples/
// C Library - <locale.h> tutorialspoint.com/c_standard_library/locale.h.htm

#include <stdio.h>
#include <stdlib.h>	// stdlib.h contains the exit method.
#include <time.h>	// External libraries not covered in class, but allowed for lab1.
#include <locale.h>	// External libraries not covered in class, but allowed for lab1.

// Using values provided from lab. Hardcoded for convenience here in lab1, but reading these as inputs from a 
// user-editable file or files wouldn't be too difficult.
#define FICATaxRate .042  	
#define FICALimit 110100

// ------------------------------------------------------------------------------------------------------
// PLACE SECONDARY FUNCTIONS BEFORE MAIN IN C.
// This input validation function returns an int which will be the inputCompletionFlag for certain operations.
int charInputValidation()	// Booleans are not in C. Typically 1 (an int) is used for true, 0 is false.
{
	int returnValue;		// Value to be returned.
	int validInputFlag = 1;	// Value which determines if we break this function's loop.
	char charInput;			// Value inputted by user.
	
	while (validInputFlag == 1)
	{

		scanf("%c", &charInput);	// Not 100% sure why '&' used here.

		if (charInput == 'Y' || charInput == 'y')
		{
			returnValue = 1; 		// Essentially returns a yes.
			validInputFlag = 0;		// Breaks function loop.

		} else if (charInput == 'N' || charInput == 'n')
		{
			returnValue = 0;		// Essentially returns a no.
			validInputFlag = 0; 	// Breaks function loop.
		} else if (charInput != 'Y' && charInput != 'y' && charInput != 'N' && charInput != 'n' && charInput != '\n')
		{   // '\n' checked in the above if statement b/c scanf also reads your enter key. 
			printf("Incorrect input format. Type Y for yes or N for no.\n"); // Incorrect input.
		}
	}
	return returnValue;		// Returns the value (1 is y, 0 is n)
}
// ------------------------------------------------------------------------------------------------------

int main()
{
	// --------------------------------------------------------------------------------------------------
	// FIRST, WE DO FILE OPERATIONS----------------------------------------------------------------------
	FILE *outFile;				// File pointers are referred to as "handles".

	char fileOutName[21];		// Actual input path and file name.

	printf("Enter the name & path of the file to hold the results [max 20 characters]: ");
	scanf("%s", fileOutName);

	// Now, we don't want to overwrite an existing file by accident:
	if ((outFile = fopen(fileOutName, "r")) != NULL) // Returns a value if there is a file there.
	{
		// Following code b/c opening existing file in write mode ("w") clears the existing file.
		printf("\nA file by the name %s exists.\n", fileOutName);
		printf("Do you wish to overwrite it? (Y or N): \n");
		int anotherInputCompletionFlag = 1;
		anotherInputCompletionFlag = charInputValidation(); // Returns 1 for yes, 0 for no.
		
		// Assuming no, we don't want to overwrite:
		if (anotherInputCompletionFlag == 0) 
		{
			fclose(outFile); // Should close file first.
			printf("\nProgram aborted to prevent overwrite of existing file!");
			exit(1);
		}
	}
	// Assuming yes, we either want to overwrite or open a new file:
	outFile = fopen(fileOutName, "w"); // Opens text file for output. Deletes any existing file.
		
	if (outFile == NULL) // If user provided bad/empty input to output file question.
	{
		printf("Could not create the output file! Program terminating.\n");
		exit(1);
	}

	// --------------------------------------------------------------------------------------------------
	// NEED TO DO HEADER OPERATIONS BEFORE FIRST INPUT AND SUBSEQUENT INPUT LOOP-------------------------

	// NEW <time.h> CODE FOR HEADER. EXTERNAL LIBRARY LEARNED FROM ONLINE RESOURCES CITED AT TOP OF FILE:
	time_t dateAndTime;
	struct timeStruct * timeTime;
	dateAndTime = time(NULL);
	timeTime = localtime(&dateAndTime);

	fprintf(outFile, "%39s", "ACME");				// Note: fprintf for printing to the opened file.
	fprintf(outFile, "%56s", asctime (timeTime));	// asctime returns pointer to string from struct.

	// Set up locale.h for local currency formatting:
	setlocale(LC_NUMERIC, "");

	// Now write column headers to file:
	fprintf(outFile, "%s", "\n");
	fprintf(outFile, "%-4s", "Line No.");  	// Supposed to be right justified 4 print positions? (- is for left justi)
	fprintf(outFile, "%12s", "Emp. No.");	// Supposed to be right justified 6 print positions? 
	fprintf(outFile, "%8s", "Given");
	fprintf(outFile, "%14s", "Surname");
	fprintf(outFile, "%17s", "YTD ($)");
	fprintf(outFile, "%19s", "Gross ($)");
	fprintf(outFile, "%15s", "FICA Tax\n"); 

	// --------------------------------------------------------------------------------------------------

    // STRATEGY:
	// I could put employee details in a struct and then create an array of that struct, but it's unnecessary and
	// I don't know how many entries/employees that I may need to enter. I'll just print/write to a file instead.
	// THE FOLLOWING VARIABLES ARE EACH OF OUR EMPLOYEE DETAILS:
	int employeeNumber;
	char employeeGivenName[15];	// First name.
	char employeeSurName[20];	// Last name.
	float YTD, Gross, FICATaxAmount;

	// THE FOLLOWING VARIABLE KEEPS COUNT OF WHICH LINE WE'RE INPUTTING ON:
	int lineNumber = 0;

	// THE FOLLOWING VARIABLES ARE RUNNING TOTALS FOR THE FOOTER.
	float totalGross = 0; float totalFICA = 0;
	
	
	// Always accepts input for one employee once file is opened.
	int inputCompletionFlag = 1;	// Controls whether the main loop will continue collecting user input.

	// --------------------------------------------------------------------------------------------------
	// This is the actual main loop that will determine if we continue to accept user input for employee data:
	while (inputCompletionFlag == 1)
	{
		// Increment line counter:
		lineNumber++;

		// First we collect data about each inputted employee:
		printf("Input Employee Number:\n");
		scanf("%d", &employeeNumber);

		printf("Input Employee First Name:\n");
		scanf("%s", employeeGivenName);

		printf("Input Employee Last Name:\n");
		scanf("%s", employeeSurName);

		printf("Input Employee YTD:\n");
		scanf("%f", &YTD);

		printf("Input Employee Gross:\n");
		scanf("%f", &Gross);

		// Then we do the tax calculations:
		if (YTD >= FICALimit)						// PAY NO TAX.
		{
			FICATaxAmount = 0; 
		} else if (YTD + Gross <= FICALimit)		// PAY TAX ON TOTAL GROSS.
		{
			FICATaxAmount = Gross * FICATaxRate;
		} else										// PAY TAX ONLY ON (FICALIMIT - YTD)
		{
			FICATaxAmount = (FICALimit - YTD) * FICATaxRate;
		}

		// Update the running totals:
		totalGross = totalGross + Gross;
		totalFICA = totalFICA + FICATaxAmount;

		// Now, write all of this data to the opened file with correct formatting:
		fprintf(outFile, "%s", "\n");
		fprintf(outFile, "%-4d", lineNumber);  				// Supposed to be right justified 4 print positions? 
		fprintf(outFile, "%14d     ", employeeNumber);		// Supposed to be right justified 6 print positions? 
		fprintf(outFile, "%-12s", employeeGivenName);		// (- is for left justified)
		fprintf(outFile, "%-12s", employeeSurName);			// (is right justified by default)
		fprintf(outFile, "%4s $%'11.2f", "", YTD);			// ' symbol for commas once locale.h set
		fprintf(outFile, "%4s $%'9.2f", "", Gross);
		fprintf(outFile, "%4s $%'9.2f", "", FICATaxAmount); // */


		// Check if user wants to input another employee, else end loop and begin program termination:
		printf("Input data for another employee? Enter \"Y\" or \"N\"\n");
		inputCompletionFlag = charInputValidation();		// Returns 1 for yes, 0 for no.
	}
	// BEFORE CLOSING, WRITE FOOTER WITH RUNNING TOTALS TO FILE.
	fprintf(outFile, "\n\n%58s", "Totals");				
	fprintf(outFile, "%10s $%'9.2f", "", totalGross);
	fprintf(outFile, "%4s $%'9.2f", "", totalFICA);

	// NEED TO CLOSE FILE FIRST BEFORE EXITING
	printf("Closing your saved output file and terminating program. Have a nice day.\n");
	fclose(outFile);
	exit(1);
}
// ------------------------------------------------------------------------------------------------------
