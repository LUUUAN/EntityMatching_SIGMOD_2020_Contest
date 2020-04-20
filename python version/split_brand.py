import re
import dataset


def postfix_split(brand, model, postfixes):
    """
        考虑某型号的后缀将其分离。如1d和1dx
    """

    postfix_dict = {model: []}

    for postfix in postfixes:
        postfix_dict[model+postfix] = []
    product_list = dataset.model_index[brand][model]
    for product in product_list:
        has_postfix = False
        for postfix in postfixes:
            reg_ext = re.compile(model[0] + r'[ ]?' + model[1] + r'[- ]?' + postfix, re.I)
            if reg_ext.search(dataset.all_data[product].get('<page title>')):
                postfix_dict[model+postfix].append(product)
                has_postfix = True
                break
        if not has_postfix:
            postfix_dict[model].append(product)


    for new_model, model_list in postfix_dict.items():
        dataset.model_index[brand][new_model] = model_list



def generation_split(brand, model, *spec_generations):
    """
        Split generations I, II, III, IV
    """

    product_list = dataset.model_index[brand][model]
    generation_dict = {model: []}
    generation_transfer = [(4, 'IV'),  (3, 'III'), (2, 'II')]
    for generation in generation_transfer:
        # print(generation[1]) # IV III II
        generation_dict[model + ' MARK ' + generation[1]] = []
    for spec_generation in spec_generations:
        generation_dict[model + ' ' + spec_generation] = []


    for product in product_list:
        product_page_title = dataset.all_data[product].get('<page title>')
        is_spec = False
        for spec_generation in spec_generations:
            reg_ext = re.compile(spec_generation, re.I)
            if reg_ext.search(product_page_title):
                generation_dict[model + ' ' + spec_generation].append(product)
                is_spec = True
                break
        if is_spec:
            continue
        has_generation = False
        for generation in generation_transfer:
            # print(generation)
            reg_ext = re.compile('((MARK)|(MK))[ ]?(({})|({}))'
                                 .format(generation[0], generation[1]), re.I)
            if reg_ext.search(product_page_title):
                generation_dict[model + ' MARK ' + generation[1]].append(product)
                has_generation = True
                break
        if not has_generation:
            generation_dict[model].append(product)
    # file.close()

    for new_model, model_list in generation_dict.items():
        dataset.model_index[brand][new_model] = model_list



def split_brand():
    postfix_split('Canon', '1D', ['X', 'C', 'S'])
    postfix_split('Canon', 'G7', ['X'])
    generation_split('Canon', '1D', 'Mark II N')
    generation_split('Canon', '1DS')
    generation_split('Canon', '5D')
    generation_split('Canon', '7D')
    generation_split('Canon', 'G1')

