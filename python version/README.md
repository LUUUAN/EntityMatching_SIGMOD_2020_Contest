# 2020 SIGMOD Progrmming Contest

#### DBGroup@SUSTech

## Task

This is the source code (python version) for [2020 SIGMOD Programming Contest](http://www.inf.uniroma3.it/db/sigmod2020contest/task.html). We aim to use a Rule-based method to solve the entity resolution problem in a real-world dataset.

## Getting Started

1. Install Python 3 and Pandas if you haven't yet.

2. Download and unzip this project
3. Run 'main.py' through command line inside the project folder. The command line argument should include:
   1. Path of input dataset folder (2013_camera_specs)
   2. Path of output csv file

```
$python3 main.py <path of input dataset> <path of output csv file>
```

##### Example:

```
$python3 main.py ./2013_camera_specs ./submission.csv
```



## Algorithm

The sketch of our solution consists of the following steps:

### Brand Indexing

Using the 'brand' attribute in json file to obtain a list of brand appeard in the dataset.

### Model Indexing

Design a rule-based method to seperate out the model name of each camera specification under each brand. Assign each specification one or more model name.

### Filtering

Design filters for several brand to revise their model name.

### Consturting Invert List

Using reverted index to find manifestations that refers to more than 2 models. They are likely to be accerssories.

### Addressing "Same Model Different Representation" Issue

- Solve the problem that one model can have multiple model names. 
- For a brand, if two model names shares more than 2 products, merge these two models together.
  - www.ebay.com//46420, Canon PowerShot ELPH 300 HS IXUS 220 HS 12 1 MP Digital Camera Black | eBay
  - "ELPH 300 HS" and "IXUS 220 HS" refer to the same camera

### Addressing Unclassified Manifestation

Use rule to resolve remaining manifestation.

- The product not blocked into any model will be add to the "others" list.
- Extract products in "others" according to their page title using prefix as auxiliary data

### Model Merging

Merge models name and corresponding manifestations that refer to the same real world model.

- Map each model name to a “standardized” name.
  - E.g. (in Casio)
    - Map "EX-FH20" to "FH20"
    - Map "EX FH20" to "FH20"

### Different Model Splitting

Split models that have same prefix and different postfix.

- e.g. Canon ID Mark I & Canon 1D Mark II

