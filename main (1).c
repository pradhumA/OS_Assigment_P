#include<unistd.h>
#include<time.h>
#include<stdio.h>
#include<stdlib.h>
int pagereplacement(int k, int pages[],int n)
{

      int frame[k];
      for(int i=0; i<k; i++)
      {
        frame[i] = -1;
      }
    int faults=0, hit=0;
    int len_frame =  k; 
    int len = len_frame;

    for(int i=0; i<n; i++)
    {
        int count = 0;
        for(int j=0; j<len_frame; j++)
        {
            if(pages[i] == frame[j])
            {
                count++;
            }
        }
        if(count != 0)
        {
            hit++;
        }
        else
          {
            if(len >= len_frame)
              {
                len = 0;
              }
              frame[len] = pages[i];
              len++;
              faults++;
          }
    }
    printf("\n no. of frames : %d      | faults : %d    hits   : %d\n", k, faults, hit);
}
int main()

{
  int pages[15];
 srand(time(NULL)); 
for(int i=0;i<15;i++)
{
    pages[i] = rand()%9;
}
printf("Page Frames are : ");
for(int i=0;i<15;i++)
{
  printf("%d", pages[i]);
}
  for(int k=1; k<8; k++)
  {
    pagereplacement(k, pages, sizeof(pages)/sizeof(int));
  }
printf("%ld", sizeof(pages)/sizeof(int));
}