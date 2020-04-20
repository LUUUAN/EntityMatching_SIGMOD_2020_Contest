import dataset

ban_dict = {
    'Canon': {'4l', '8f', 'e1', 'e3', 'e7', 'efs 18', 'eos-1d', 'is 24', 'lens 18',
              'lens 55', 'quantaray 18', 'f3', 's 18', 's 55', 'sigma 18', 'sigma 70',
              'slr 10', 'stm 18', 'tamron 18', 'v1', 'usm 75'},
    'Fujifilm': {'f1', 'p1', 'v1'},
    'Magnavox': {'F7'},
    'Nikon': {'af 18', 'af 70', 'f1', 'f3', 'f4', 'f5', 'nikkor 18', 'nikkor 35', 'nikkor 55',
              'nikon 16', 'p1', 'sigma 70', 'u1', 'v1', 'v2', 'vr10', 'y1'},
    'Olympus': {'e1', 'f2', 'ihs 12', 'ihs 14', 'olympus 14', 'p1', 'v2'},
    'Panasonic': {'f2', 'f3', 'lumix 14'},
    'Pentax': {'al 18', 'da 18', 'dal 18', 'l 18'},
    'Philips': {'F7'},
    'Sony': {'and 18', 'b 16', 'class 10', 'digital 20', 'e 18', 'f1', 'f2', 'f3', 'p1',
             'pz 16', 'sony 16', 'sony18', 'v1'}
}


def filter_ban_list():
    # Delete damn model names.
    for brand in dataset.model_index:
        if brand in ban_dict:
            del_list = ban_dict[brand]
            for del_item in del_list:
                del_item = del_item.upper()
                if del_item in dataset.model_index[brand].keys():
                    del dataset.model_index[brand][del_item]
                else:
                    # print("NO MODEL \"{}\" in BRAND \"{}\"".format(del_item, brand))
                    pass


def filter_Dahua():
    brand = 'Dahua'
    models = dataset.model_index[brand]
    del_list = []
    for model in models:
        if len(model) <= 4 or model.find(' ') != -1:
            del_list.append(model)
    for del_model in del_list:
        del dataset.model_index[brand][del_model]


def filter_Hikvision():
    brand = 'Hikvision'
    models = dataset.model_index[brand]
    del_list = []
    for model in models:
        if model.find('-') == -1 or model[0] == 'I':
            del_list.append(model)
    for del_model in del_list:
        del dataset.model_index[brand][del_model]


def filter_Sigma():
    brand = 'Sigma'
    models = dataset.model_index[brand]
    del_list = []
    for model in models:
        if model[0:2] != 'DP' and model[0:2] != 'SD':
            del_list.append(model)
    for del_model in del_list:
        del dataset.model_index[brand][del_model]


def filter_SVP():
    brand = 'SVP'
    models = dataset.model_index[brand]
    del_list = []
    for model in models:
        if model[0:3] == '500':
            del_list.append(model)
    for del_model in del_list:
        del dataset.model_index[brand][del_model]


def filter_Kodak():
    brand = 'Kodak'
    models = dataset.model_index[brand]
    del_list = []
    for model_name, model_list in models.items():
        for model in model_list:
            page_title = dataset.all_data[model].get('<page title>')
            if page_title.find('Printer Dock') != -1 or \
                    page_title.find('2 Digital Cameras with') != -1:
                del_list.append((model_name, model))
    for del_model_name, del_model in del_list:
        dataset.model_index[brand][del_model_name].remove(del_model)


def filter_Kodak():
    brand = 'Kodak'
    models = dataset.model_index[brand]
    del_list = []
    for model_name, model_list in models.items():
        for model in model_list:
            page_title = dataset.all_data[model].get('<page title>')
            if page_title.find('Printer Dock') != -1 or \
                    page_title.find('2 Digital Cameras with') != -1:
                del_list.append((model_name, model))
    for del_model_name, del_model in del_list:
        dataset.model_index[brand][del_model_name].remove(del_model)


def filter_Panasonic():
    brand = 'Kodak'
    models = dataset.model_index[brand]
    del_list = []
    for model_name, model_list in models.items():
        for model in model_list:
            page_title = dataset.all_data[model].get('<page title>')
            if page_title.find('flash kit for') != -1:
                del_list.append((model_name, model))
    for del_model_name, del_model in del_list:
        dataset.model_index[brand][del_model_name].remove(del_model)


def filtering():
    print("Filtering the specific brand")
    filter_ban_list()
    filter_Dahua()
    filter_Hikvision()
    filter_Sigma()
    filter_SVP()
    filter_Kodak()
    filter_Panasonic()




