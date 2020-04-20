import dataset

E = 2



def intersection():
    print("Merge same models with different representations")
    deleted_set = set()
    for brand in dataset.model_index.keys():
        print("Merge brand:", brand)
        model_items = list(dataset.model_index[brand].items())
        for i in range(0, len(model_items)):
            if model_items[i][0] in deleted_set:
                continue
            for j in range(i+1, len(model_items)):
                if model_items[j][0] in deleted_set:
                    continue
                if len(set(model_items[i][1]) & set(model_items[j][1])) > E:
                    # If two model have more than 2 intersection json instances, merge these two models
                    dataset.model_index[brand][model_items[i][0]] += \
                        dataset.model_index[brand][model_items[j][0]]
                    print("Merge:", model_items[j][0], model_items[i][0])
                    del dataset.model_index[brand][model_items[j][0]]
                    deleted_set.add(model_items[j][0])






