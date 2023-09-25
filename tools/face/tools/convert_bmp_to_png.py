import os
from PIL import Image

def convert_bmp_to_png(input_dir, output_dir):
    # 遍历目录中的文件
    for filename in os.listdir(input_dir):
        # 构建输入文件路径和输出文件路径
        input_path = os.path.join(input_dir, filename)
        output_path = os.path.join(output_dir, os.path.splitext(filename)[0] + ".png")

        # 检查文件是否为 BMP 图像
        if os.path.isfile(input_path) and filename.endswith(".bmp"):
            try:
                # 打开并转换图像格式
                with Image.open(input_path) as img:
                    img.save(output_path, "PNG")
                print(f"Convert {filename} to PNG successfully.")
            except Exception as e:
                print(f"Failed to convert {filename}: {e}")

# 指定输入目录和输出目录
input_directory = "../bin/data/cluster"
output_directory = "2500/"

# 调用函数进行转换
convert_bmp_to_png(input_directory, output_directory)
