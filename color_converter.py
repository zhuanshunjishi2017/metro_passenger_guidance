def rgb24_to_rgb16(r, g, b):
    # 将24位颜色转换为16位
    r_16 = (r >> 3) & 0x1F # 红色取高5位
    g_16 = (g >> 2) & 0x3F # 绿色取高6位
    b_16 = (b >> 3) & 0x1F # 蓝色取高5位

    # 合并为16位颜色值
    rgb_16 = (r_16 << 11) | (g_16 << 5) | b_16
    return rgb_16
def on_hex_change(hex_str):
    # 解析十六进制颜色码
    if hex_str.startswith('#'):
        hex_str = hex_str[1:]
    if len(hex_str) == 6:
        r = int(hex_str[0:2], 16)
        g = int(hex_str[2:4], 16)
        b = int(hex_str[4:6], 16)
        if 0 <= r <= 255 and 0 <= g <= 255 and 0 <= b <= 255:
            return [r, g ,b]
        else:
            print("无效输入")
            return
    else:
        print("无效输入")
        return
while 1:
    input_string = input("输入24位色的16进制码(按e退出)：")
    if input_string == "e":
        break
    rgblist = on_hex_change(input_string)
    if not rgblist:
        continue
    r, g, b = rgblist[0], rgblist[1], rgblist[2]
    rgb_16 = rgb24_to_rgb16(r, g, b)
    print(f"十六位色: {hex(rgb_16)}")
