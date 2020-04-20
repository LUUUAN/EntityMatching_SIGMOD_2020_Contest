import sys
import time
from dataset import *
from index_brand import index_brand
from index_model import index_model
from filters import filtering
from reverse_index import build_reverse_index
from reverse_index import multiple_model
from intersection import intersection
from merge_same import merge_same
from split_brand import split_brand
from resolve_others import resolve_others



def solve(output_path):
    result = {'left_spec_id': [], 'right_spec_id': []}
    for brand, brand_items in model_index.items():
        for model, model_items in brand_items.items():
            for i in range(0, len(model_items)):
                for j in range(i + 1, len(model_items)):
                    result['left_spec_id'].append(model_items[i])
                    result['right_spec_id'].append(model_items[j])
    df = pd.DataFrame(result, columns=['left_spec_id', 'right_spec_id'])
    df.to_csv(output_path, index=False)


if __name__ == '__main__':
    if len(sys.argv) != 3:
        print('WRONG ARGUMENTS, run \' python main.py path/of/input/dataset path/of/output/csv/file\'')
        sys.exit(1)
    input_path = sys.argv[1]
    output_path = sys.argv[2]
    print("Begin Entity Resolution")

    start_time = time.localtime()

    read_all(input_path)
    index_brand()
    index_model()
    filtering()  # Filter on each brand separately, mainly delete useless model names
    build_reverse_index()
    multiple_model()
    intersection()
    collect_remain()

    resolve_others()

    merge_same()
    split_brand()

    #model_index_to_file('./model')  # 解注释可以打印到文件

    solve(output_path)

    #os.system("python ./judge/judge.py "+output_path)

    print("Start time: ", end=" ")
    print(time.strftime("%H:%M:%S", start_time))
    end_time = time.localtime()
    print("End time : ", end=" ")
    print(time.strftime("%H:%M:%S", end_time))
