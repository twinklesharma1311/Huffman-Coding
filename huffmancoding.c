/* HUFFMAN ENCODING Implementation in C */
/*The program takes a string from user as input and encodes it for transmission*/
#include<stdio.h>
#include<conio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
/*STRUCTURES*/
struct tree                  //The structure is used to build the binary tree.
{   int data;                //Variable 'data' is used to represent the frequency of each character
    int trav_val;            //trav_value takes 0 if the node is traversed to the right or takes 1 if it is traversed to the left
    char ckey;               //This is the character assigned to the leaf node
   struct tree *rlink;             //Right child
    struct tree *llink;             //Left child
}**root=NULL,*temp;

struct calc_sum              //The structure is used to store the sum of two min values
{   int sum;                 //Sum is the total value
    int right;               //right is the biggest of the two min values
    int left;                //left is the smallest of the two min values
}*total;

struct string                //The structure is used to assign the frequency of each character
{   char alph;               //alph is the character
    int freq;                //freq is the frequency of the character
}*ori_string,*pre_string;    /*ori_string is the original string(containing freq and char of each character entered by user)
			       and pre_string is the preserved string*/

struct encode                //The structure is used to assign the Huffman's code to different characters
{   char letter;             //This is the character assigned to the leaf node
    int *code;               //It contains the Huffman's code for the character
    int length;              //Contains the length of the Huffman's code
}*enc;

/*FUNCTION DECLARATIIONS*/
void sort(struct string [],int);
void insert(struct calc_sum [],int);
void display(struct tree *);
void encode(struct encode *,char *);

/*GLOBAL VARIABLES*/
/*i,j,k,m are loop variables
  next_char is the no of different characters in the input string
  str_len is the length of the input string
  ch is used to drive the menu
  input is the input string
*/
int next_char,str_len;
static int *trav,m,root_check,i,j,k;
char ch,*input;

/*MAIN*/
void main()
{   do
    {   char *string;//It is a temporary pointer variable containing the string
	int flag,char_freq;//char_freq is the freq of different characters
	clrscr();
	printf("\nEnter the String\n");
	fflush(stdin);
	input=gets(string);
	str_len=strlen(input);
	printf("\nLength of the string:%d",str_len);
	ori_string=(struct string *)malloc(str_len*sizeof(struct string));
	trav=(int *)malloc(str_len*sizeof(int));
	pre_string=(struct string *)malloc(str_len*sizeof(struct string));
	for(i=32,next_char=0;i<127;i++)
	{       flag=0,char_freq=0,j=0;
	    while(input[j]!='\0')
	    {   if(input[j]==i)
		{   flag=1;
		    ori_string[next_char].alph=pre_string[next_char].alph=input[j];
		    char_freq++;
		}
		j++;
	    }
	    if(flag)
	    {
		ori_string[next_char].freq=pre_string[next_char].freq=char_freq;
		next_char++;
	    }
	}
	if(next_char==1||str_len==0)
	{   printf("\n!!!ENCODING NOT POSSIBLE!!!\nYOU HAVE ENTERED\n1.A SINGLE CHARACTER OR\n2.SAME SET OF CHARACTERS");
	    printf("\nPRESS 'ENTER' TO TRY AGAIN");
	    fflush(stdin);
	    getch();
	}
	else
	{   enc=(struct encode *)malloc(next_char*sizeof(struct encode));
	    printf("\n\nPRESS ENTER TO GET THE FREQUENCIES\n");
	    getch();
	    clrscr();
	    printf("\nCharacter\tFrequency\n");
	    for(i=0;i<next_char;i++)
	    printf("%c\t\t %d\n",pre_string[i].alph,pre_string[i].freq);
	    sort(ori_string,next_char);
	}
	printf("\n\nDo you want to continue(Y/N)\n");
	free(ori_string);
	free(pre_string);
	free(trav);
	free(total);
	free(temp);
	free(root);
	fflush(stdin);
	free(enc);
	scanf("%c",&ch);
    }while(ch=='y'||ch=='Y');
}

/*
  Name of Routine     :  Sort
  Type/Classified as  :  Sorting
  Function Call       :  sort() from main
  Parameters Passed   :  ori_string,next_char
  Return Type         :  Void
  Purpose             :  Sorts the frequencies in decreasing order
			 Swap is used for swapping values
			 k stores the no. of sum values
*/
void sort(struct string ori_string[],int next_char)
{  int swap;
k=next_char-1;

    total=(struct calc_sum *)malloc(k*sizeof(struct calc_sum));
    printf("\nPRESS ENTER TO GET THE SUM\n");
	    getch();
	    clrscr();
	    printf("\nLeft\tRight\tTotal\n");
   while(k>0)
       {
	for(i=0;i<k+1;i++)
	for(j=i+1;j<k+1;j++)
	{   if(ori_string[i].freq<ori_string[j].freq)
	    {   swap=ori_string[i].freq;
		ori_string[i].freq=ori_string[j].freq;
		ori_string[j].freq=swap;
	    }
	}

		total[next_char-1-k].left=ori_string[i-1].freq;
	total[next_char-1-k].right=ori_string[i-2].freq;
	total[next_char-1-k].sum=total[next_char-1-k].left+total[next_char-1-k].right;
	ori_string[k-1].freq=total[next_char-1-k].sum;
	printf("%d  \t%d  \t%d\n",total[next_char-1-k].left,total[next_char-1-k].right,total[next_char-1-k].sum);
	k--;
    }
    insert(total,next_char);
}

/*
  Name of Routine     :  Insert
  Type/Classified as  :  Insertion
  Function Call       :  insert() from sort()
  Parameters Passed   :  total,next_char;
  Return Type         :  Void
  Purpose             :  Constructs a binary tree using the sum and the frequency values
			 Variables rflag and lflag are used to identify a new root
			 Variables lchange and rchange are used to compress two roots into 1
			 tot_roots calcultes the no of roots possible and integer value is assigned to roots using the power function
			 roots for max roots that a tree can contain
			 count gives the no. of roots at any time
			 swap for swapping two characters and k stores the no of sum values
*/
void insert(struct calc_sum total[],int next_char)
{   int roots,count=0,lflag,rflag,rchange=0,lchange=0;
    char swap;
    float tot_roots;
    struct tree *l;
    k=0;
tot_roots=log(2*next_char-1)/log(2);
roots=pow(2,tot_roots-1);
root=(struct tree **)malloc(roots*sizeof(struct tree));
    for(i=0;i<roots;i++)
    root[i]=NULL;
    while(k<next_char-1)
    {   lflag=rflag=1;
	temp=(struct tree *)malloc(sizeof(struct tree));
	temp->data=total[k].sum;
	if(k==0)
	{   struct tree *r=(struct tree *)malloc(sizeof(struct tree));
	    r->data=total[k].right;
	    r->trav_val=1;
	    r->rlink=NULL;
	    r->llink=NULL;
	    for(i=next_char-1;i>=0;i--)
	    {   if(pre_string[i].freq==total[k].right)
		{   r->ckey=pre_string[i].alph;
		    pre_string[i].freq=0;
		    break;
		}
	    }
	    temp->rlink=r;
	    l=(struct tree *)malloc(sizeof(struct tree));
	    l->data=total[k].left;
	    l->trav_val=0;
	    l->rlink=NULL;
	    l->llink=NULL;
	    for(i=next_char-1;i>=0;i--)
	    {   if(pre_string[i].freq==total[k].left)
		{   l->ckey=pre_string[i].alph;
		    pre_string[i].freq=0;
		    break;
		}
	    }
	    temp->llink=l;
	    root[0]=temp;
	}
	else
	{   for(i=0;i<roots;i++)      // check for right
	    {   if(total[k].right==root[i]->data)
		{
		    root[i]->trav_val=1;
		    temp->rlink=root[i];
		    rflag=0;
		    rchange=i;
		    root[rchange]=temp;
		}
		if(!rflag)
		break;
	    }
	    if(i>=roots)
	    {   struct tree *r=(struct tree *)malloc(sizeof(struct tree));
		r->data=total[k].right;
		r->trav_val=1;
		r->rlink=NULL;
		r->llink=NULL;
		for(i=next_char-1;i>=0;i--)
		{   if(pre_string[i].freq==total[k].right)
		    {       r->ckey=pre_string[i].alph;
			pre_string[i].freq=0;
			break;
		    }
		}
		temp->rlink=r;
	    }
	    for(i=0;i<roots;i++)       //check for left
	    {   if(total[k].left==root[i]->data)
		{
		    root[i]->trav_val=0;
		    temp->llink=root[i];
		    lflag=0;
		    lchange=i;
		    root[lchange]=temp;
		}
		if(!lflag)
		break;
	    }
	    if(i>=roots)
	    {
		struct tree *l=(struct tree *)malloc(sizeof(struct tree));
		l->data=total[k].left;
		l->trav_val=0;
		l->rlink=NULL;
		l->llink=NULL;
		for(i=next_char-1;i>=0;i--)
		{   if(pre_string[i].freq==total[k].left)
		    {   l->ckey=pre_string[i].alph;
			pre_string[i].freq=0;
			break;
		    }
		}
		temp->llink=l;
	    }
	    if(rflag&&lflag)
	    {   root[++count]=temp;
		if(root[count]->llink->data==root[count]->rlink->data)
		{
		    swap=root[count]->llink->ckey;
		    root[count]->llink->ckey=root[count]->rlink->ckey;
		    root[count]->rlink->ckey=swap;
		}
	    }
	    if(!rflag&&!lflag)
	    root[rchange]=NULL;
	}
      k++;
    }//while
    printf("\nPRESS ENTER TO GET THE HUFFMAN CODES\n");
    getch();
    clrscr();
    printf("\nHUFFMAN CODES\n");
    m=0,i=0,j=0,k=0;
    display(root[lchange]);
    printf("\nPRESS ENTER TO GET THE ENCODED STRING\n");
    getch();
    clrscr();
    encode(enc,input);
}

/*
  Name of Routine     :  Display
  Type/Classified as  :  I/O
  Function Call       :  disp() from insert()
  Parameters Passed   :  root[lchange]
  Return Type         :  Void
  Purpose             :  Displays the Huffcode for each character
			 root_check is used to check if the current node is the root
			 trav[] is an array which stores the huffman code for each character
*/
void display(struct tree *disp)
{   if(disp!=NULL)
    {   if(root_check>=1)
	trav[m++]=disp->trav_val;
	root_check++;
	display(disp->llink);
	display(disp->rlink);
	if(disp->llink==NULL&&disp->rlink==NULL)
	{   enc[i].code=(int *)malloc(m*sizeof(int));
	    printf("%c-",disp->ckey);
	    enc[i].letter=disp->ckey;
	    enc[i].length=0;
	    for(m=0;m<root_check-1;m++)
	    {          printf("%d",trav[m]);
		       enc[i].code[j]=trav[m];
		       enc[i].length++;
		       j++;
	    }
	    i++;
	    j=0;
	    printf("\n");
	}
	m--;
	root_check--;
    }
}

/*
  Name of Routine     :  Encode
  Type/Classified as  :  I/O
  Function Call       :  encode() from insert()
  Parameters Passed   :  *enc,*input
  Return Type         :  Void
  Purpose             :  Displays the encoded string
*/
void encode(struct encode *enc,char *input)
{    printf("\nEncoded String is:\n");
     for(i=0;i<str_len;i++)
     {  j=0;
	while(j<next_char)
	{   if(enc[j].letter==input[i])
	    for(k=0;k<enc[j].length;k++)
	    printf("%d",enc[j].code[k]);
	    j++;

	}
     }

}
