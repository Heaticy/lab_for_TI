import serial
import threading
import tkinter as tk
import matplotlib.pyplot as plt

plt.rcParams["font.sans-serif"] = ["SimHei"]  # 设置字体为SimHei
plt.rcParams["axes.unicode_minus"] = False  # 设置支持负号

from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
from matplotlib.figure import Figure
import numpy as np

global spectrum_window, spectrum_data, fs
spectrum_window = None

spectrum_data = np.array([])


class SpectrumWindow(tk.Toplevel):
    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)
        self.geometry("1000x800")  # 设置窗口大小为1000x800
        self.title("频谱")
        self.protocol("WM_DELETE_WINDOW", self.on_window_close)  # 监听窗口关闭事件

        self.figure = Figure(figsize=(10, 8), dpi=100)  # 增大图形大小
        self.axes1 = self.figure.add_subplot(211)  # 增加subplot(211)
        self.axes2 = self.figure.add_subplot(212)  # 增加subplot(212)

        self.canvas = FigureCanvasTkAgg(self.figure, master=self)
        self.canvas.draw()
        self.canvas.get_tk_widget().pack()
        self.ser = args[0].ser  # 获取串口对象

    def on_window_close(self):
        self.ser.write(b"spectrum_closed\n")  # 向串口发送"spectrum_closed"命令
        self.destroy()  # 关闭窗口

    def update_spectrum(self, data):
        self.axes1.clear()
        self.axes1.set_title("1通道频谱")  # 设置第一个图的标题
        self.axes2.clear()
        self.axes2.set_title("2通道频谱")  # 设置第二个图的标题
        fs = 2560000
        freqline = np.linspace(0, fs, 1024)
        self.axes1.plot(freqline, data[:1024])  # 在第一个子图中画前1024个数据点
        self.axes1.set_xlabel("Frequency(Hz)")
        self.axes2.plot(freqline, data[1024:])  # 在第二个子图中画后1024个数据点
        self.axes2.set_xlabel("Frequency(Hz)")
        self.canvas.draw()


class SweepApp(tk.Tk):
    def __init__(self, port="COM3", baudrate=115200, timeout=1):
        super().__init__()
        self.ser = serial.Serial(port, baudrate, timeout=timeout)
        self.create_widgets()
        self.create_serial_thread()

    def create_widgets(self):
        self.button_frame = tk.Frame(self, height=200)
        self.button_frame.pack(side=tk.TOP, fill=tk.X)

        self.single_freq_button = tk.Button(
            self.button_frame, text="单频", command=self.single_freq, padx=50, pady=20
        )
        self.single_freq_button.pack(side=tk.LEFT, expand=True)

        self.scan_freq_button = tk.Button(
            self.button_frame, text="扫频", command=self.scan_freq, padx=50, pady=20
        )
        self.scan_freq_button.pack(side=tk.RIGHT, expand=True)

        self.text = tk.Text(self)
        self.text.pack()

    def create_serial_thread(self):
        self.serial_thread = threading.Thread(target=self.read_from_serial)
        self.serial_thread.daemon = True
        self.serial_thread.start()

    def read_from_serial(self):
        global spectrum_data, spectrum_window
        while True:
            data = self.ser.readline()
            if data:
                decoded_data = data.decode().strip()
                self.text.insert(tk.END, decoded_data + "\n")
                self.text.see(tk.END)
                if decoded_data == "test":
                    self.ser.write(b"test\n")
                if spectrum_window is not None:
                    try:
                        spectrum_data = np.append(spectrum_data, float(decoded_data))
                        if len(spectrum_data) >= 2048:
                            spectrum_window.update_spectrum(spectrum_data)
                            spectrum_data = np.array([])
                    except ValueError:
                        pass
                # added code for log sweep
                elif "log_sweep_window" in dir(self) and self.log_sweep_window:
                    try:
                        spectrum_data = np.append(spectrum_data, float(decoded_data))
                        if len(spectrum_data) >= 20:
                            freqs = np.logspace(
                                np.log10(1e3), np.log10(800e3), 20
                            )  # 生成对数扫频的频率点
                            self.log_sweep_window.update_spectrum(freqs, spectrum_data)
                            spectrum_data = np.array([])

                    except ValueError:
                        pass

    def single_freq(self):
        self.ser.write(b"single_sweep_open\n")  # 发送"single_sweep_open\n"到串口
        SingleFreqWindow(self)

    def scan_freq(self):
        self.log_sweep_window = LogSweepWindow(self)
        self.ser.write(b"double_sweep_open\n")  # 发送"double_sweep_open\n"到串口


class SingleFreqWindow(tk.Toplevel):
    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)
        self.geometry("800x600")  # 设置窗口大小
        self.title("单频")
        self.protocol("WM_DELETE_WINDOW", self.on_window_close)  # 监听窗口关闭事件
        self.fs = 2000000

        entry_frame = tk.Frame(self)
        entry_frame.place(relx=0.5, rely=0.2, anchor=tk.CENTER)

        tk.Label(entry_frame, text="频率:").pack(side=tk.LEFT)
        self.entry = tk.Entry(entry_frame, width=20)  # 可以通过width参数控制输入框的长度
        self.entry.pack(side=tk.LEFT)

        self.start_scan_button = tk.Button(
            self, text="开始扫描", command=self.start_scan, padx=50, pady=20
        )
        self.start_scan_button.place(relx=0.5, rely=0.5, anchor=tk.CENTER)

        self.ser = args[0].ser  # 获取串口对象

    def on_window_close(self):
        self.ser.write(b"single_sweep_closed\n")  # 发送"single_sweep_closed"到串口
        self.destroy()  # 关闭窗口

    def start_scan(self):
        global spectrum_window, spectrum_data, fs
        spectrum_data = np.array([])
        # 在这里添加开始扫描的逻辑
        freq = self.entry.get()
        if freq:
            freq_message = freq.encode() + b"\n"  # 转为bytes类型
            self.ser.write(freq_message)
            freq = int(freq)
            if freq > 500000:
                fs = 3600000
            elif freq > 200000:
                fs = 2000000
            elif freq > 100000:
                fs = 1000000
            elif freq > 50000:
                fs = 500000
            elif freq > 20000:
                fs = 200000
            elif freq > 10000:
                fs = 100000
            elif freq > 5000:
                fs = 50000
            elif freq > 2000:
                fs = 20000
            elif freq > 1000:
                fs = 10000
            elif freq > 500:
                fs = 5000
            elif freq > 200:
                fs = 2000
            else:
                fs = 1000
        spectrum_window = SpectrumWindow(self)


class LogSweepWindow(tk.Toplevel):
    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)
        self.geometry("800x600")  # 设置窗口大小
        self.title("对数扫频")
        self.protocol("WM_DELETE_WINDOW", self.on_window_close)  # 监听窗口关闭事件

        self.figure = Figure(figsize=(10, 8), dpi=100)  # 增大图形大小
        self.axes = self.figure.add_subplot(111)  # 添加一个subplot

        self.canvas = FigureCanvasTkAgg(self.figure, master=self)
        self.canvas.draw()
        self.canvas.get_tk_widget().pack()

        self.ser = args[0].ser  # 获取串口对象

    def on_window_close(self):
        self.ser.write(b"sweep_closed\n")  # 向串口发送"sweep_closed"命令
        self.destroy()  # 关闭窗口

    def update_spectrum(self, freqs, data):
        self.axes.clear()
        self.axes.set_title("对数扫频结果")  # 设置图的标题
        self.axes.plot(freqs, data)  # 画扫频结果
        self.axes.set_xlabel("Frequency(Hz)")
        self.axes.set_ylabel("Amplitude")
        self.canvas.draw()


if __name__ == "__main__":
    app = SweepApp("COM17", 921600, 1)
    app.title("扫描仪")
    app.mainloop()
