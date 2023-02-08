import os
import glob
import shutil
import random
import argparse
from labelme2cocoInstance import labelme2coco
from mask_to_labelme import gt_to_labelMe

def create_dir(path):
    if not os.path.exists(path):
        os.makedirs(path)

def temp_copy(json_path, img_path, temp_path):
    for i, json_file in enumerate((json_path)):
        shutil.copy2(json_file, temp_path)

    for i, img_file in enumerate(img_path):
        shutil.copy2(img_file, temp_path)

def split_test_train(json_path, img_path, temp_path, dirname):
    temp_copy(json_path, img_path, temp_path)
    temp_dir = glob.glob(f"{dirname}/temp_data/*", recursive=True)
    temp_file_list = []

    for temp_file in temp_dir:
        name = temp_file.split("/")[-1].split(".")[0]
        temp_file_list.append(name)
    temp_file_list = list(set(temp_file_list))
    test_file_rand = random.sample(temp_file_list, k=0)

    for copy_file in temp_dir:
        name = copy_file.split("/")[-1].split(".")[0]
        if name in test_file_rand:
            shutil.copy2(copy_file, test_path)
        else:
            shutil.copy2(copy_file, train_path)

def test_data_to_coco_folder(test_path, dirname):
    files_prep = glob.glob(os.path.join(test_path, "*"))
    data_path_test = f"{dirname}/data/test"
    name_extention = []
    for file_path in files_prep:
        extension = file_path.split("/")[-1].split(".")[-1]
        filename = file_path.split("/")[-1].split(".")[0]
        if extension != "json":
            name_extention.append([filename, extension])
    for x in name_extention:
        make_img_path = f"{dirname}/data_prep/test/{x[0]}.{x[1]}"
        make_json_path = f"{dirname}/data_prep/test/{x[0]}.json"
        shutil.copy2(make_img_path, data_path_test)

def train_data_to_coco_folder(train_path, dirname):
    files_prep = glob.glob(os.path.join(train_path, "*"))
    data_path_train = f"{dirname}/data/train"
    name_extention = []
    for file_path in files_prep:
        extension = file_path.split("/")[-1].split(".")[-1]
        filename = file_path.split("/")[-1].split(".")[0]
        if extension != "json":
            name_extention.append([filename, extension])
    for x in name_extention:
        make_img_path = f"{dirname}/data_prep/train/{x[0]}.{x[1]}"
        make_json_path = f"{dirname}/data_prep/train/{x[0]}.json"
        shutil.copy2(make_img_path, data_path_train)

if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        description="labelme annotation to coco data json file."
    )
    parser.add_argument(
        "labelme_images_test",
        help="Directory to labelme images and annotation json files.",
        type=str,
    )
    parser.add_argument(
        "labelme_images_train",
        help="Directory to labelme images and annotation json files.",
        type=str,
    )

    parser.add_argument(
        "current_dir",
        help="Directory to labelme images and annotation json files.",
        type=str,
    )
    parser.add_argument(
        "data_set", help="provide the name of the dataset folder. eg: dataset_BRUMAS_v1"
    )
    
    args = parser.parse_args()
    dirname = args.current_dir
    data_set_name = args.data_set

    with open (f"{dirname}/{data_set_name}/classesNumber.txt", "r") as classesNumber:
      classes = classesNumber.read().splitlines()

    image_path = f"{dirname}/{data_set_name}/done"
    instance_path = f"{dirname}/{data_set_name}/instance"

    create_dir(instance_path)
    
    '''GT to LabelMe'''
    gt_to_labelMe(image_path, classes, instance_path, data_set_name)

    img_path = glob.glob(os.path.join(dirname, data_set_name, "done", "*"))
    json_path = glob.glob(os.path.join(dirname, data_set_name, "instance", "*"))
    gt_path = glob.glob(os.path.join(dirname, data_set_name, "instance", "*"))



    temp_path = f"{dirname}/temp_data"
    test_path = f"{dirname}/data_prep/test"
    train_path = f"{dirname}/data_prep/train"
    prer_data = f"{dirname}/data_prep"
    save_test = "./data/test/test.json"
    save_train = "./data/train/train.json"
    final_test = f"{dirname}/data/test"
    final_train = f"{dirname}/data/train"

    '''create necessary dir'''
    create_dir(f"{dirname}/temp_data")
    create_dir(f"{dirname}/data_prep")
    create_dir(f"{dirname}/data_prep/train")
    create_dir(f"{dirname}/data_prep/test")
    create_dir(f"{dirname}/data/train")
    create_dir(f"{dirname}/data/test")



    '''splite test and train'''
    split_test_train(json_path, img_path, temp_path, dirname)
    print("split_test_train done ")

    '''delete temp dir'''
    shutil.rmtree(temp_path)
    print("temp dir deleted")

    '''copying test image to coco folder'''
    test_data_to_coco_folder(test_path, args.current_dir)
    print("imgData for test data done")
    train_data_to_coco_folder(train_path, args.current_dir)
    print("imgData for train data done")

    '''converting labelme data to coco'''    
    labelme_json_test = glob.glob(os.path.join(args.labelme_images_test, "*.json"))
    labelme_json_train = glob.glob(os.path.join(args.labelme_images_train, "*.json"))
    labelme2coco(labelme_json_test, save_test)
    print("test data converted to coco")
    labelme2coco(labelme_json_train, save_train)
    print("train data converted to coco")

    '''delete prep dir'''
    shutil.rmtree(prer_data)
    print("prep dir deleted")
