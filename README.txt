Producing preliminary statistics:
---------------------------------

Suppose fairytales directory contains all the corpora to be used for training.
Now we produce a wordlist and a statistics file:

cat fairytales/* | ./tokenizer -l | sort | uniq > fairytales.words

cat fairytales/* | ./tokenizer -l | ./wordstats > fairytales.stats


Producing a training/test set:
------------------------------

For this example we choose to create a network with 1000 hidden1 units and 600
hidden2 units.

First, check the wordlist size with wc:

wc -l fairytales.words

It may be that it is too big, and you want to restrict the corpora only to words
occurring more than a few (20) times.

So, create the restricted wordlist:

cat fairytales/* | ./tokenizer -l -f fairytales.stats -t 20 | sort | uniq > wordpredict-h1000-H600-0001.words

Check restricted vocabulary size:

wc -l wordpredict-h1000-H600-0001.words

Repeat this step if not satisfied.

Also check how many sentences are you going to extract with this treshold
setting:

cat fairytales/* | ./tokenizer -l -f fairytales.stats -t 20 | grep '^\.$' | wc -l
1128

Now create the training, validation, test datasets. We are going to split the
1128 sentences in 650 for training, 230 for validation, and the remaining
(248) for testing:

cat fairytales/* | ./tokenizer -l -f fairytales.stats -t 20 -n 650 -s0 | ./makedataset -n 3 -f wordpredict-h1000-H600-0001.words > training.dat
cat fairytales/* | ./tokenizer -l -f fairytales.stats -t 20 -n 230 -s 650 | ./makedataset -n 3 -f wordpredict-h1000-H600-0001.words > validation.dat
cat fairytales/* | ./tokenizer -l -f fairytales.stats -t 20 -s 880 | ./makedataset -n 3 -f wordpredict-h1000-H600-0001.words > testing.dat


Training the network:
---------------------

For training, we specify a desired error with -d, a maximum number of epochs
with -e, and the network parameters (-h, -H).

We use the training file for training, and the validation file to check for
overfitting:

./train -f wordpredict-h1000-H600-0001.net -F validation.dat -d 0.005 -e 200 -h 1000 -H 600 training.dat

Now watch the classification error decrease, and the validation error too.
As soon as the validation error starts rising, you should stop training to
avoid overfitting.

