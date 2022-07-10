#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define FILE_NAME  "student_data.bin"


typedef struct
{
    int rollno;
    char sname[50];
    float cgpa;
    int marks[8][3];
} srec;

srec stdinfo[25];
int stdno = 0;


//Head message
void headMessage(const char *message)
{
    printf("\n---------------------------------------------------------------------------\n");
    printf("%s",message);
    printf("\n---------------------------------------------------------------------------");

}

//int limitcheck

//valid marks
int validsub_marks(){
       int marks;
        printf("Enter valid subject marks= ");
        scanf("%d",&marks);

    return marks;
}
//Validate name
int isNameValid(const char *name)
{
    int validName = 1;
    int len = 0;
    int index = 0;
    len = strlen(name);
    for(index =0; index <len ; ++index)
    {
        if(!(isalpha(name[index])) && (name[index] != '\n') && (name[index] != ' '))
        {
            validName = 0;
            break;
        }
    }
    return validName;
}

// Add student in list
void addStudent()
{
    srec rec = {0};

    FILE *fp = NULL;
    fp = fopen(FILE_NAME,"ab+");
    if(fp == NULL)
    {
        printf("\nNot able to open file\n");
        exit(1);
    }

    headMessage("ADD NEW STUDENT");

    printf("\nENTER YOUR DETAILS BELOW:");
    printf("\n---------------------------------------------------------------------------\n");
    printf("\nStudent Roll Number  =\n");

    fflush(stdin);
    scanf("%d",&(rec.rollno));
    printf("\nStudent Name  =\n");
    fflush(stdin);
    scanf("%s",(rec.sname));


    for(int i=0;i<8;i++)
    {
    	printf("\nEnter Student marks in sem %d for 3 subjects\n",i+1);
    	printf("\nEnter -1 for marks of each subject if not available\n");
    	scanf("%d",&rec.marks[i][0]);
    	if (rec.marks[i][0]>100){
                while(rec.marks[i][0]>100){
           rec.marks[i][0]=validsub_marks();
                }
    	}
    	scanf("%d",&rec.marks[i][1]);
    	if (rec.marks[i][1]>100){
               while(rec.marks[i][0]>100)
           rec.marks[i][1]=validsub_marks();
    	}
    	scanf("%d",&rec.marks[i][2]);
    	if(rec.marks[i][2]>100){
    	    while(rec.marks[i][0]>100)
           rec.marks[i][2]=validsub_marks();
    	}
    }

    printf("\nEnter Student CGPA  =\n");
    fflush(stdin);
    scanf("%f",&rec.cgpa);
     if(rec.cgpa>10){
            while(rec.cgpa>10){
        printf("enter valid cgpa");
        scanf("%f",&rec.cgpa);
            }

     }

    printf("details of %d are written into file",rec.rollno);
    fwrite(&rec,sizeof(rec), 1, fp);
    fclose(fp);
}


// search student
void searchStudent()
{
    int found = 0;
    int rn;
    srec rec = {0};

    FILE *fp = NULL;
    fp = fopen(FILE_NAME,"rb");
    if(fp == NULL)
    {
        printf("\nNot able to open file\n");
        exit(1);
    }

    headMessage("SEARCH STUDENT");

    printf("\nEnter Student Roll number to search:");
    fflush(stdin);
    scanf("%d",&rn);

    while(fread(&rec, sizeof(rec), 1, fp))
    {
    	if(rec.rollno == rn)
        {
            found = 1;
            break;
        }
    }

    if(found)
    {
        printf("\nStudent Roll Number = %d",rec.rollno);
        printf("\nStudent Name = %s",rec.sname);
        for(int i=0;i<8;i++)
        {

	     printf("\nSem %d marks",i);
	     for(int j=0; j<3; j++)
	     {
		if(rec.marks[i][j]==-1)
		{
		     printf(" -- ");
		}
		else
		{
		     printf(" %d ",rec.marks[i][j]);
		}

	     }
        }
        printf("\nCGPA = %f",rec.cgpa);

    }
    else
    {
        printf("\nNo Record");
    }
    fclose(fp);
    printf("\n\nPress any key to go to main menu.....");
    fflush(stdin);
    getchar();
}


// view students function
void viewStudent()
{
    int found = 0;
    srec rec = {0};
    unsigned int rec_count = 1;

    FILE *fp = NULL;
    fp = fopen(FILE_NAME,"rb");
    if(fp == NULL)
    {
        printf("File is not opened\n");
        exit(1);
    }

    headMessage("VIEW STUDENT DETAILS");

    while (fread (&rec, sizeof(rec), 1, fp))
    {
    	printf("\n\nStudent Roll Number = %d",rec.rollno);
    	printf("\nStudent Name = %s",rec.sname);
    	printf("\n");
    	for(int i=0;i<8;i++)
        {

	     printf("\nSem %d marks",i+1);
	     for(int j=0; j<3; j++)
	     {
            if(rec.marks[i][j]==-1)
            {
		      printf(" -- ");
            }
            else
            {
		      printf(" %d ",rec.marks[i][j]);
            }
	     }
        }
        printf("\n");
    	printf("\nCGPA = %f",rec.cgpa);
    	found = 1;
        ++rec_count;
        printf("\n\n\n");
    }
    fclose(fp);
    if(!found)
    {
        printf("\nNo Record");
    }
    printf("\n\nPress any key to go to main menu.....");
    fflush(stdin);
    getchar();
}

// Delete student entry
void deleteStudent()
{
    int found = 0;
    int rn = 0;
    srec rec = {0};

    FILE *fp = NULL;
    fp = fopen(FILE_NAME,"rb");
    if(fp == NULL)
    {
        printf("File is not opened\n");
        exit(1);
    }

    FILE *tmpFp = NULL;
    tmpFp = fopen("tmp.bin","wb");
    if(tmpFp == NULL)
    {
        fclose(fp);
        printf("File is not opened\n");
        exit(1);
    }

    headMessage("DELETE STUDENT DETAILS");

    printf("\nEnter Student Roll no. for delete:");
    scanf("%d",&rn);

    while (fread (&rec, sizeof(rec), 1, fp))
    {
        if(rec.rollno != rn)
        {
            fwrite(&rec,sizeof(rec), 1, tmpFp);
        }
        else
        {
            found = 1;
        }
    }

    (found)? printf("\nRecord deleted successfully....."):printf("\nRecord not found");
    fclose(fp);
    fclose(tmpFp);
    remove(FILE_NAME);
    rename("tmp.bin",FILE_NAME);
}

//function to update student record
void updateStudent()
{
    int rn,found=0;
    srec rec;

    FILE *fp = NULL;
    fp = fopen(FILE_NAME,"rb+");
    if(fp == NULL)
    {
        printf("File is not opened\n");
        exit(1);
    }

    headMessage("UPDATE RECORD");

    printf("\nEnter roll no of the Student to be updated\n");
    scanf("%d",&rn);

    while(fread(&rec, sizeof(rec), 1, fp))
        {

            if(rec.rollno == rn)
            {
                found = 1;
                break;
            }
        }
        if(found)
        {
        	char c;
        	printf("\nEnter 'y' if name change is required, 'n' else not required to change\n");
        	fflush(stdin);
        	scanf("%c",&c);
        	if(c == 'y')
            {
                printf("\nEnter student's updated name  =\n");
                fflush(stdin);
                scanf("%s",(rec.sname));
            }

        	for(int i=0;i<8;i++)
        	{

        	   printf("\nEnter 'y' if change required in %d sem, 'n' else not required to change\n",i+1);
        	   fflush(stdin);
               scanf("%c",&c);

                  if(c == 'y')
                  {
                     printf("\nEnter student's updated marks in sem %d for 3 subjects\n",i+1);
        	         printf("\nEnter -1 for marks if not available\n");
        	         scanf("%d %d %d",&rec.marks[i][0],&rec.marks[i][1],&rec.marks[i][2]);
                  }
                  else continue;
        	}

        	printf("\nEnter 'y' if cgpa change is required, 'n' else not required to change\n");
        	fflush(stdin);
        	scanf("%c",&c);
        	if(c=='y')
            {
                printf("\nEnter student's updated CGPA  =\n");
        	    fflush(stdin);
        	    scanf("%f",&rec.cgpa);
            }

        	fseek(fp,-sizeof(srec),SEEK_CUR);
        	fwrite(&rec,sizeof(rec),1,fp);
        	printf("\nStudent Record Updated Successfully\n");
        }
        else
        {
            printf("\nNO RECORD\n");
        }
    fclose(fp);

    fflush(stdin);
    printf("\nPress any key to continue\n");
    getchar();

}

void readdata()
{
	srec rec = {0};

	FILE *fp = NULL;
	fp = fopen(FILE_NAME,"rb");
	if(fp == NULL)
	{
	   printf("File is not opened\n");
	   exit(1);
	}

	while (fread (&rec, sizeof(rec), 1, fp))
	{
		stdinfo[stdno].rollno = rec.rollno;
		strcpy(stdinfo[stdno].sname, rec.sname);
		stdinfo[stdno].cgpa = rec.cgpa;
		for(int i=0;i<8;i++)
		{
			stdinfo[stdno].marks[i][0] = rec.marks[i][0];
			stdinfo[stdno].marks[i][1] = rec.marks[i][1];
			stdinfo[stdno].marks[i][2] = rec.marks[i][2];
		}

		stdno++;
	}

	fclose(fp);
}

void merge(srec newstudent[],int start, int mid, int end)
{
    int a,b,k=0;
    srec temp[end-start+ 1];
    a = start;
    b = mid + 1;
	while(a<=mid && b<=end)
	{
	    if(newstudent[a].cgpa < newstudent[b].cgpa)
	    {
	        temp[k] = newstudent[b];
	        k++;
	        b++;
	    }
	    else if(newstudent[a].cgpa > newstudent[b].cgpa)
	    {
	        temp[k] = newstudent[a];
	        k++;
	        a++;
	    }
	}
	while(a<=mid)
	{
	    temp[k] = newstudent[a];
	    k++;
	    a++;
	}
	while(b<=end)
	{
	        temp[k] = newstudent[b];
	        k++;
	        b++;
	}
	k = 0;
	for(int c = start; c<=end;c++)
	{
	    newstudent[c] = temp[k];
	    k++;
	}
}



void mergesort(srec newstudent[],int start, int end )
{
    int mid;
    if(start < end)
    {
        {
        	mid = (start + end)/2;
        	mergesort(newstudent,start,mid);
        	mergesort(newstudent,(mid + 1),end);
        	merge(newstudent,start,mid,end);
        }
    }
}


void generatemarklist()
{
	readdata();
	mergesort(stdinfo,0,stdno);

	FILE *fp = NULL;
	fp = fopen("output.txt","w");
	if(fp == NULL)
	{
	   printf("\nNot able to open file\n");
	   exit(1);
	}


    fprintf(fp,"%s  %s    %s\n","S.No","Roll No","CGPA");
    printf("%s  %s    %s\n","S.No","Roll No","CGPA");

	for(int i=0;i<stdno;i++)
	{
        fprintf(fp,"\n%d      %d      %f\n",i+1,stdinfo[i].rollno,stdinfo[i].cgpa);
        printf("\n%d      %d      %f\n",i+1,stdinfo[i].rollno,stdinfo[i].cgpa);
	}
	fclose(fp);
	printf("\nGrade List Generated Successfully !!\n");

    fflush(stdin);
    printf("\nPress any key to continue\n");
    getchar();
}

//Display menu
void menu()
{
    int choice = 0;
    do
    {
        headMessage("MAIN MENU");
        printf("\n\n\n1.Add Student");
        printf("\n2.Search Student");
        printf("\n3.View Student");
        printf("\n4.Delete Student");
        printf("\n5.Update Student");
        printf("\n6.Grade List");
        printf("\n0.Exit");
        printf("\n\n\nEnter choice => ");
        scanf("%d",&choice);
        printf("\n");
        switch(choice)
        {
        case 1:
            addStudent();
            break;
        case 2:
            searchStudent();
            break;

        case 3:
            viewStudent();
            break;

        case 4:
            deleteStudent();
            break;

        case 5:
            updateStudent();
            break;

        case 6:
            generatemarklist();
            break;
        case 0:
            printf("\n\n\n\t\t\t\tThank you!!!\n\n\n\n\n");
            exit(1);
            break;
        default:
            printf("\n\n\n\t\t\tINVALID INPUT!!! Try again...");
        }                                            //Switch Ended
    }
    while(choice!=0);
}


//Check file exist or not
int isFileExists(const char *path)
{
    // Try to open file
    FILE *fp = fopen(path, "rb");
    int status = 0;
    // If file does not exists
    if (fp != NULL)
    {
        status = 1;
        // File exists hence close file
        fclose(fp);
    }
    return status;
}
void init()
{
    FILE *fp = NULL;
    int status = 0;
    status = isFileExists(FILE_NAME);
    if(!status)
    {
        //create the binary file
        fp = fopen(FILE_NAME,"wb");
        fclose(fp);
    }
}

int main()
{
	setvbuf(stdout, NULL, _IONBF, 0);
	init();
    menu();
    // welcomeMessage();
    // login();
    return 0;
}

