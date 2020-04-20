import re
import os
import csv
import dataset

prefix_dict = {
    'Canon': ['IXUS', 'ELPH', 'IXY', 'Powershot', 'SX', 'EOS', 'SD', 'S', 'A', 'G'],
    'Casio': ['EX', 'QV'],
    'Fujifilm': ['Finepix', 'X', 'mini'],
    'GE': [],
    'Olympus': [r'SP', r'E', r'TG', r'SZ', r'Stylus', r'x', r'XZ', r'TG', r'SH'],
    'Panasonic': ['DMC'],
    'Samsung': ['EC'],
    'Sony': ['NEX', 'DSC', 'DMC', 'DSLR', 'SLT'],
    'Nikon': ['Nikon']
}

meaningful_postfix_dict = {
    'Fujifilm': r'(s|t|w)',
    'Sony': r'(r|s|(M3)|(II)|(III)|(IV))',
    'Nikon': r'(s|x|e|h)',
    'Canon': r'i'
}

auxiliary_same_dict = {
    'Panasonic': {('FT25', 'TZ25', 'ZS15'), ('FT4', 'TS4'), ('TZ18', 'ZS8'), ('TZ20', 'ZS10'),
                  ('TZ30', 'ZS20'), ('TZ40', 'ZS30'), ('TZ55', 'ZS25'), ('TZ7', 'ZS3'), ('TZ10', 'ZS7'),
                  ('TZ60', 'ZS40')},
    'Canon': {('IXUS 155', 'ELPH 150'), ('IXUS 510', 'ELPH 530'), ('IXUS 140', 'ELPH 130'),
              ('IXUS 145', 'ELPH 135'), ('IXUS 150', 'ELPH 140'), ('IXUS 240', 'ELPH 320'),
              ('IXUS 265', 'ELPH 340'), ('IXUS 1100', 'ELPH 510'), ('IXUS 310', 'ELPH 500'),
              ('IXUS 40', 'ELPH SD300'), ('IXUS 85', 'ELPH SD770'), ('IXUS 90', 'ELPH SD790'),
              ('IXUS 110', 'ELPH SD960'), ('IXUS 210', 'ELPH SD3500'), ('IXUS 700', 'ELPH SD500'),
              ('IXUS 800', 'ELPH SD700'), ('IXUS 850', 'ELPH SD800'), ('IXUS 900', 'SD900'),
              ('IXUS 950', 'ELPH SD850'), ('IXUS 960', 'SD960'), ('IXUS 970', 'SD890'),
              ('IXUS 980', 'SD990'), ('IXUS 1000', 'SD4500'), ('DS6041', '300D'), ('DS126191', '1000D'),
              ('X50', '1100D'), ('DS126311', '600D'), ('A3400', 'A34000'), ('IXUS i5', 'SD20'),
              ('IXUS i7', 'SD40'), ('EOS1', '1D')},
    'Fujifilm': {('JX520', 'JX500'), ('HS30', 'HS33'), ('HS25', 'HS28')},
    'Sony': {('RX100', 'RX100M3')}
}


def refine_model(brand_name, model_raw):
    model_clean = model_raw
    prefix_reg_list = prefix_dict[brand_name]
    if brand_name in meaningful_postfix_dict.keys():
        meaningful_postfix_reg = meaningful_postfix_dict[brand_name]
    else:
        meaningful_postfix_reg = None
    for reg in prefix_reg_list:
        if len(reg) == 1:
            prefix_ext = re.compile(r'^' + reg + r'[- ](.*)', re.I)

        else:
            prefix_ext = re.compile(r'^' + reg + r'[- ]?(.*)', re.I)
        if prefix_ext.findall(model_clean):
            tmp = prefix_ext.findall(model_clean)[0]
            if not re.match('[0-9]+[a-zA-Z]*$', tmp):
                model_clean = tmp
            else:
                model_clean = reg + tmp  # 去掉空格和-
    if re.match('[a-zA-Z]+[ -][0-9]+$', model_clean, re.I):
        tmp = re.findall('([a-zA-Z]+)[ -]([0-9]+)$', model_clean)
        model_clean = tmp[0][0] + tmp[0][1]
    if re.match('[a-zA-Z]+[0-9]+[a-zA-Z]+', model_clean):
        if not (meaningful_postfix_reg and re.match(r'.*' + meaningful_postfix_reg + r'$', model_raw, re.I)):
            model_clean = re.findall('([a-zA-Z]+[0-9]+)[-a-zA-Z]+', model_clean)[0]
    return model_clean.upper()



def merge(to_model, from_model, brand):
    dataset.model_index[brand][to_model] += dataset.model_index[brand][from_model]
    dataset.model_index[brand][to_model] = list(set(dataset.model_index[brand][to_model]))
    del dataset.model_index[brand][from_model]


def merge_same():

    for brand, brand_items in dataset.model_index.items():
        if brand not in prefix_dict.keys():
            continue
        model_dict = {}
        for model, model_items in brand_items.items():
            refined_model = refine_model(brand, model)
            if refined_model not in model_dict:
                model_dict[refined_model] = []
            model_dict[refined_model].append(model)

        # 合并打表的merg
        # filter_Kodak()
        # filter_Panasonic()e
        if brand in auxiliary_same_dict.keys():
            same_models = auxiliary_same_dict[brand]
            for same_model_tup in same_models:
                merge_list = []
                for item in same_model_tup:
                    tmp = refine_model(brand, item)
                    if tmp in model_dict.keys():
                        merge_list.append(tmp)
                    # print(item, refine_model(brand, item) in model_dict)
                # print(merge_list)
                if len(merge_list) > 1:
                    dest = merge_list[0]
                    for i in range(1, len(merge_list)):
                        model_dict[dest] = model_dict[dest] + model_dict[merge_list[i]]
                        print(model_dict[dest])
                        del model_dict[merge_list[i]]

        for item in model_dict.items():
            same_list = item[1]
            # 以最短的model名作为合并标准
            min_length = len(same_list[0])
            min_idx = 0
            for i in range(1, len(same_list)):
                if len(same_list[i]) < min_length:
                    min_length = len(same_list[i])
                    min_idx = i
            tmp = same_list[min_idx]
            same_list[min_idx] = same_list[0]
            same_list[0] = tmp
            if len(same_list) > 1:
                for i in range(1, len(same_list)):
                    print('Merge:', same_list[i], 'to', same_list[0])
                    merge(same_list[0], same_list[i], brand)
