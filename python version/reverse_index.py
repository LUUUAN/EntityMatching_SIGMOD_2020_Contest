import dataset


def build_reverse_index():
    for brand, brand_items in dataset.model_index.items():
        for model_name, model_items in brand_items.items():
            for item in model_items:
                if item not in dataset.reverse_index.keys():
                    dataset.reverse_index[item] = set()
                dataset.reverse_index[item].add((brand, model_name))


def get_more_than_three():
    for spec_id, models in dataset.reverse_index.items():
        if len(models) >= 3:
            print(spec_id, models)


def multiple_model():
    del_list = []
    for spec_id, brand_models in dataset.reverse_index.items():
        if len(set(brand_models)) >= 3:
            for brand, model in brand_models:
                dataset.model_index[brand][model].remove(spec_id)
            del_list.append(spec_id)
    for del_id in del_list:
        del dataset.reverse_index[del_id]
