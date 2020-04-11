# 2020 SIGMOD Progrmming Contest

#### DBGroup@SUSTech

## Task
This is the source code for 2020 SIGMOD Programming Contest. We aim to use a Rule-based method to solve the entity resolution problem in a real-world dataset.

## Getting Started

1. Environment Requirement:
   
1. CMake (minimum version required: 3.14)
   
1. Download this project and redirect to the project folder

3. Input the path of dataset folder during reproduction process:

   ```
   $cmake .
   $make
   $./sigmod_contest <path of input dataset folder>
   ```

   Example:

   ```
   $cmake .
   $make
   $./sigmod_contest ./2013_camera_specs
   ```


4. The result "submission.csv" file will be generated under current folder

## Algorithm

This project use the following steps to realize entity resolution.

### Read data

Read the dataset and store the 'page_title' and 'brand' attribute into a map.

```
<specification_id, (page_title, brand)>
```

e.g.

```
<www.ebay.com//23857, (Kodak Eashyshare Z980 | eBay, Kodak)>
```



### Index Brand

- Record a list of brand from the 'brand' attribute read from the 'Read data' step
- Filtering the illegal brand. (e.g. brand name with > 2 words)
- Blocking each data according to brand.
- Stored in a map <brand, unordered_set<string>>
- Using string edit distance to merge same brand.
  - Referenced from https://leetcode-cn.com/problems/edit-distance/solution/bian-ji-ju-chi-by-leetcode-solution/

### Index Model

Design a rule-based method to seperate out the model name of each camera specification under each brand. 

Do blocking based on the page_title and the model extracted from it. A product can appear in different blocks. (more than 1 model name could be extracted from a page_title)

### Reverse Index

- Build reverse index and delete some special product from blocks.
  - E.g. "Nikon MH 63 Battery Charger Coolpix s 202 S200 S220 S230 S500 S510 S700 S 018208096251 | eBay"

### Deal with Same Model Different Representation Problem

- Solve the problem that one model can have multiple model names. 
- For a brand, if two model names shares more than 2 products, merge these two models together.
  - www.ebay.com//46420, Canon PowerShot ELPH 300 HS IXUS 220 HS 12 1 MP Digital Camera Black | eBay
  - "ELPH 300 HS" and "IXUS 220 HS" refer to the same camera

### Extract Models from Others

Use rule to resolve remaining manifestation.

- The product not blocked to any model will be add to the “others” list.
- Extract products in others according to their page title using prefix as auxiliary data.

### Merge Model

Merge models name and corresponding manifestations that refer to the same real world model.

- Build a hash function, map each model name to a “standardized” name.
  - E.g. (in Casio)
    - Map "EX-FH20" to "FH20"
    - Map "EX FH20" to "FH20"

### Split Different Model

Split models that have different postfix 

- e.g. Canon ID Mark I & Canon 1D Mark II
