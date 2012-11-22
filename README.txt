Command line arguments for tokenizer:
-------------------------------------

-l          convert every word to lowercase
-n <num>    maximum number of sentences to output
-s <num>    number of sentences to skip at beginning
-t <tresh>  minimum word frequency treshold
            (sentences having a word below treshold will not be output)


Producing a commonWords.txt file:
---------------------------------

Let's say you want a treshold of 20:

 cat text/* | ./scripts/buildDictionary.sh -l -t 20 > commonWords.txt


Producing a stats.txt file:
---------------------------

 cat text/* | ./scripts/printStats.sh -l -t 20 > stats.txt


Producing a training/test set:
------------------------------

 cat text/* | ./scripts/makeTrainingFile.sh -l -t 20 -n ## -s ## > filename.dat
 add the header MANUALLY to filename.dat

 remember to use THE SAME TRESHOLD for commonWords.txt, stats.txt and ther
 training file
