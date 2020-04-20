from edit_distance import *
import dataset

columns_df = ['id', '<page title>']
brand_list = []
brand_items = {}

special_brand = ['Hikvision', 'Dahua', 'Konica', 'Cannon', 'Coolpix', 'Vista Quest', 'Go Pro']
ban_list = {'Tamron', 'SHOOT'}


def valid_brand_test(brand):
    if brand == 'unknown':
        return False
    temp = brand.split(" ")
    if len(temp) > 2:
        return False
    for word in temp:
        for alphabet in word:
            if not alphabet.isalpha():
                return False
    return True


def remove_duplicate():
    for brand in brand_list:
        temp = brand.split(" ")
        if len(temp) > 1 and temp[0] in brand_items:
            brand_list.remove(brand)


def remove_low_frequency():
    threshold = len(brand_items)
    del_list = []
    for brand, item_list in brand_items.items():
        if len(item_list) <= threshold * 0.01:
            del_list.append(brand)
        else:
            brand_list.append(brand)
    for del_item in del_list:
        del brand_items[del_item]


def add_special():
    for brand in special_brand:
        brand_list.append(brand)


def get_brand_list():
    for product_id, product_json in dataset.all_data.items():
        brand = product_json.get('brand')
        if brand:
            if isinstance(brand, list):
                brand = brand[0]
            if not valid_brand_test(brand):
                continue
            if brand in ban_list:
                continue
            if brand not in brand_items:
                brand_items[brand] = []
            brand_items[brand].append(product_id)

    remove_low_frequency()
    remove_duplicate()
    add_special()


def match(brand, page_title):
    if len(brand) < 4:
        temp = page_title.split(" ")
        for word in temp:
            if word == brand:
                return True
    elif page_title.lower().find(brand.lower()) != -1:
        return True
    return False


def add_data(data, key, page_title):
    data['id'].append(key)
    data['<page title>'].append(page_title)


def is_chinese_brand(brand):
    if brand == 'Hikvision' or brand == 'Dahua':
        return False
    return True


def blocking():
    for brand in brand_list:
        visit = set()
        flag = is_chinese_brand(brand)
        # data = {'id': [], '<page title>': []}
        ids = []
        for key, json_content in dataset.all_data.items():
            page_title = json_content.get('<page title>')
            if match(brand, page_title):
                if flag and key.find('www.alibaba.com') != -1:
                    continue
                if key.find('www.ebay.com') != -1 and page_title.lower().find('lot of') != -1:
                    continue
                visit.add(key)
                ids.append(key)
        if brand in brand_items:
            for brand_item in brand_items[brand]:
                if brand_item not in visit:
                    visit.add(brand_item)
                    ids.append(brand_item)
        dataset.brand_index[brand] = ids


def merge(to_brand, from_brand):
    if from_brand in dataset.brand_index and to_brand in dataset.brand_index:
        dataset.brand_index[to_brand] += dataset.brand_index[from_brand]
        del dataset.brand_index[from_brand]



def merge_pair():
    for i in range(0, len(brand_list)):
        for j in range(i + 1, len(brand_list)):
            ed = calculate_Edit_Distance(brand_list[i], brand_list[j])
            if ed < 2 or brand_list[i].find(brand_list[j]) != -1 or brand_list[j].find(brand_list[i]) != -1:
                merge(brand_list[i], brand_list[j])
    merge('Nikon', 'Coolpix')


def index_brand():
    print("Indexing brand")
    print("Getting brand list...")
    get_brand_list()
    print("Blocking according to brand...")
    blocking()
    print("Merge same brands...")
    merge_pair()



if __name__ == '__main__':
    index_brand()
