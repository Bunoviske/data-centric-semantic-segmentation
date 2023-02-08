import numpy as np
import cv2
import argparse
import glob
import os
import base64
import labelme
import json
from itertools import chain

def create_dir(path):
    if not os.path.exists(path):
        os.makedirs(path)


def gt_to_labelMe(image_path, classes, instance_path, data_set_name):
        path_to_image = sorted(glob.glob(f"{image_path}/*"))
        all_class_labels = []
        all_image_name = []
        for i, img_path in enumerate(path_to_image):
          image_name = img_path.split("/")[-1].split(".")[0]
          image_full_name = img_path.split("/")[-1]
          all_image_name.append(image_full_name)
          gt_path = f"./{data_set_name}/gt/{image_name}_GT.png"
          #print("Now processing:", img_path)

          '''encoding image data'''
          #print("start encoding image data")
          data = labelme.LabelFile.load_image_file(img_path)
          image_data = base64.b64encode(data).decode('utf-8')
          #print("finished encoding image data")

          image = cv2.imread(gt_path, cv2.IMREAD_GRAYSCALE)
          image_np = np.array(image)
          (unique, _) = np.unique(image_np, return_counts=True)
          unique = list(unique)
          all_class_labels.append(unique)

          unique_no_bg = [x for x in unique if x != 0]
          #print("unique pixel values from gt data", unique_no_bg)

          json_str = {
      }
          json_str['version'] = '5.0.1'
          json_str['flags'] = {
      }
          shapes = []
          for i in range(len(unique_no_bg)):
              single_mask = image_np[:,:]==unique_no_bg[i]
              single_mask = (single_mask*unique_no_bg[i]).astype(np.uint8)
              _,thresh = cv2.threshold(single_mask,0,255,cv2.THRESH_BINARY)
              contours, _ = cv2.findContours(thresh, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
              pixel_value = unique_no_bg[i]
              for i, points in enumerate(contours):
                shape = {
      }
                if cv2.contourArea(points) > 50:
                    shape['label'] = ' '.join(classes[pixel_value].split(":")[-1].split(" ")[1:])
                    #shape['label'] = classes[pixel_value].split(":")[-1].split(" ")[-1]
                    shape['line_color'] = None
                    shape['fill_color'] = None
                    points = np.squeeze(points)
                    points = points.tolist()
                    shape['points'] = points
                    shape['group_id'] = None
                    shape['shape_type'] = 'polygon'
                    shape['flags'] = {
            }
                    shapes.append(shape)
                else:
                    print("Ignoring small contour. size: ", cv2.contourArea(points))
                    

          json_str['shapes'] = shapes
          json_str['lineColor'] = [0, 255, 0, 128]
          json_str['fillColor'] = [255, 0, 0, 128]
          json_str['imagePath'] = f"../done/{image_full_name}"
          json_str['imageData'] = image_data
          height, width = image.shape
          json_str['imageHeight'] = height
          json_str['imageWidth'] = width
          with open(os.path.join(instance_path, image_name + '.json'), 'w') as json_file:
              json.dump(json_str, json_file, indent=2)
        flatten_list = list(chain(*all_class_labels))
        print("all class:", set(sorted(flatten_list)))
        print(all_image_name)


if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        description="GT data to labelme annotations")
    parser.add_argument(
        "current_dir",
        help="Directory to images files.",
        type=str)
    parser.add_argument(
    "data_set",
    help="provide the name of the dataset folder. eg: dataset_BRUMAS_v1",
    type=str)
    args = parser.parse_args()

    dir_name = args.current_dir
    data_set_name = args.data_set
    image_path = f"{dir_name}/{data_set_name}/done"
    instance_path = f"{dir_name}/{data_set_name}/instance"

    with open (f"{dir_name}/{data_set_name}/classesNumber.txt", "r") as classesNumber:
      classes = classesNumber.read().splitlines()

    create_dir(instance_path)
    print("Conversion started")
    gt_to_labelMe(image_path, classes, instance_path, data_set_name)
    print("Conversion finished")
