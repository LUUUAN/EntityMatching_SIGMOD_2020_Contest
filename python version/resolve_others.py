import re
from merge_same import prefix_dict
import dataset


def resolve_others_brand(brand):

    prefix_reg_list = prefix_dict[brand]

    del_list = set()
    for product in dataset.others[brand]:
        page_title = dataset.all_data[product].get('<page title>')
        found_flag = False
        for prefix in prefix_reg_list:
            if len(prefix) == 1:
                reg_ext = re.compile(r'[ ,]' + prefix + r'[0-9]+[a-zA-Z]*', re.I)
            else:
                reg_ext = re.compile(r'[ ,]' + prefix + r'[ -]?[0-9a-zA-Z]+', re.I)
            if reg_ext.findall(page_title):
                found_list = reg_ext.findall(page_title)
                # print(found_list)
                found_str = None
                for item in found_list:
                    if re.search('[0-9]+', item):
                        found_str = item
                        break
                if not found_str:
                    continue
                found_flag = True
                model_name = found_str[1:]
                if model_name not in dataset.model_index[brand]:
                    print("New model resolved:", model_name)
                    dataset.model_index[brand][model_name] = []
                dataset.model_index[brand][model_name].append(product)
                del_list.add(product)

    for del_item in del_list:
        dataset.others[brand].remove(del_item)


def resolve_others():
    print('Resolve Others...')
    for brand in prefix_dict.keys():
        print('Resolve Others:', brand)
        resolve_others_brand(brand)
