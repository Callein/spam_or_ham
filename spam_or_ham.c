#include <stdio.h>
#include <ctype.h>
#include <string.h>

struct Word
{
   char word[100];
   int count;
   double p;
   double q;
};

struct Retset
{
   char num[10];
   double r;
};

void readTrainFile(char *filename, struct Word *words, int *count);
void readTestFile(char *filename, struct Word *spamWords, struct Retset *retset, char type);
void calcProbability(struct Word *spamWords, struct Word *hamWords, int spamCount, int hamCount);

int main(int argc, char *argv[])
{

   char *trainSpamFile = "datasets/train/dataset_spam_train100.csv";
   char *trainHamFile = "datasets/train/dataset_ham_train100.csv";
   char *testSpamFile = "datasets/test/dataset_spam_test20.csv";
   char *testHamFile = "datasets/test/dataset_ham_test20.csv";
   struct Word spamWords[10000] = {0};

   struct Word hamWords[10000] = {0};
   struct Retset spamRetset[20] = {0};
   struct Retset hamRetset[20] = {0};
   int spamCount = 0;
   int hamCount = 0;

   readTrainFile(trainSpamFile, spamWords, &spamCount);
   readTrainFile(trainHamFile, hamWords, &hamCount);

   calcProbability(spamWords, hamWords, spamCount, hamCount);

   readTestFile(testSpamFile, spamWords, spamRetset, 's');
   readTestFile(testHamFile, spamWords, hamRetset, 'h');

   // printf("%s / %f\n", spamWords[0].word, spamWords[0].p);
   // printf("%s / %f\n", spamWords[1].word, spamWords[1].p);
   // printf("%d\n", hamCount);
   // printf("%d", spamCount);

   return 0;
}

void calcProbability(struct Word *spamWords, struct Word *hamWords, int spamCount, int hamCount)
{

   for (int i = 0; i <= spamCount; i++)
   {
      int hc = 1; // 해당 단어가 햄에 한번도 나오지 않은 경우 확률이 1이 되므로, 초기값을 1로 설정
      for (int j = 0; j <= hamCount; j++)
      {
         if (spamWords[i].word == hamWords[j].word)
         {
            hc = hamWords[j].count;
            break;
         }
      }
      spamWords[i].p = (double)spamWords[i].count / (double)(spamWords[i].count + hc);
      spamWords[i].q = 1 - spamWords[i].p;
   }
}

void readTestFile(char *filename, struct Word *spamWords, struct Retset *retset, char type)
{
   struct Word words[10000] = {0};
   int count = 0;
   char tok[13];

   if (type == 's')
   {
      sprintf(tok, "sspamsubject");
   }
   else
   {
      sprintf(tok, "hhamsubject");
   }

   char str_tmp[1024];
   int n = 0;

   FILE *pFile = NULL;

   pFile = fopen(filename, "r");
   if (pFile != NULL)
   {
      fgets(str_tmp, 1024, pFile);
      int c1 = 0;
      int c2 = 0;
      while (!feof(pFile))
      {
         char temp[1024] = {
             0,
         };
         int x = 0;
         fgets(str_tmp, 1024, pFile);

         for (int i = 0; i < strlen(str_tmp); i++)
         {
            if (isalpha(str_tmp[i]))
            {
               if (isupper(str_tmp[i]))
               {
                  temp[x++] = str_tmp[i] + 32;
               }
               else
               {
                  temp[x++] = str_tmp[i];
               }
            }
            else if (str_tmp[i] == ' ')
            {
               temp[x++] = str_tmp[i];
            }
         }

         char *ptr = strtok(temp, " ");
         while (ptr != NULL)
         {
            // printf("%s", ptr);
            if (count != 0 && !strcmp(tok, ptr))
            {
               printf(" 요놈 %d\n", count);
               // if (words[0].count == 0)
               // {
               //    printf("잡았다 요놈\n");
               //    struct Word dummy[10000] = {0};
               //    memcpy(words, dummy, sizeof(dummy));
               // }
               n = 0;
               ptr = strtok(NULL, " ");
               // c1++;
            }
            else
            {
               // printf("%s\n", words[0].word);
               for (int i = 0; i <= count; i++)
               {
                  if (!strcmp((words[i]).word, ptr))
                  {
                     (words[i]).count++;
                     n = 1;
                  }
               }
               if (n != 1)
               {
                  strcpy((words[count]).word, ptr);
                  (words[count]).count++;
                  count++;
               }
               n = 0;
               ptr = strtok(NULL, " ");
               // c2++;
            }
         }
      }
      // printf("%d , %d", c1, c2);
   }

   fclose(pFile);
}

void readTrainFile(char *filename, struct Word *words, int *count)
{
   char str_tmp[1024];
   int n = 0;

   FILE *pFile = NULL;

   pFile = fopen(filename, "r");
   if (pFile != NULL)
   {
      fgets(str_tmp, 1024, pFile);
      while (!feof(pFile))
      {
         char temp[1024] = {
             0,
         };
         int x = 0;
         fgets(str_tmp, 1024, pFile);

         for (int i = 0; i < strlen(str_tmp); i++)
         {
            if (isalpha(str_tmp[i]))
            {
               if (isupper(str_tmp[i]))
               {
                  temp[x++] = str_tmp[i] + 32;
               }
               else
               {
                  temp[x++] = str_tmp[i];
               }
            }
            else if (str_tmp[i] == ' ')
            {
               temp[x++] = str_tmp[i];
            }
         }

         char *ptr = strtok(temp, " ");
         while (ptr != NULL)
         {
            if (!strcmp("spamsubject", ptr) || !strcmp("hamsubject", ptr))
            {
               n = 0;
               ptr = strtok(NULL, " ");
            }
            else
            {
               for (int i = 0; i <= (*count); i++)
               {
                  if (!strcmp((words[i]).word, ptr))
                  {
                     (words[i]).count++;
                     n = 1;
                  }
               }
               if (n != 1)
               {
                  strcpy((words[(*count)]).word, ptr);
                  (words[(*count)]).count++;
                  (*count)++;
               }
               n = 0;
               ptr = strtok(NULL, " ");
            }
         }
      }
   }
   fclose(pFile);
}
