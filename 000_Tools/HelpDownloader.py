#!/usr/bin/env python

"""
Загрузчик справки с сайта
Microsoft's Old API Help File Reborn
http://laurencejackson.com/win32
"""

from urllib.request import urlopen

#
# read_chunk(url, max_chunk)
#

def read_chunk(url, max_chunk=8192*8):
    """Генератор содержимого web-файла, для использования в инструкции for"""
    with urlopen(url) as response:
        while True:
            chunk = response.read(max_chunk)
            if chunk:
                yield chunk
            else:
                return

#
# download(url, name)
#

def download(url, name):
    """Скачивает web-файл на диск"""

    # определяем размер скачиваемого файла

    file_size = 0
    with urlopen(url) as response:
        header = response.getheader('Content-Length')
        file_size = int(header)
    print('File size: %dKB' % (file_size / 1024))

    # скачиваем файл на диск

    with open(name, 'wb') as file:
        downloaded_size = 0
        for chunk in read_chunk(url):
            downloaded_size += len(chunk)
            file.write(chunk)
            # выводим прогресс чтения файла
            status = 'Downloading... %3.2f%% complete.' % (downloaded_size * 100. / file_size)
            print(status, end='\r')
        print()

#
# main()
#

def main():
    """Точка входа в программу"""
    download('http://laurencejackson.com/win32/Win32.chm', 'Win32.chm')

#
# start script
#

if __name__ == '__main__':
    main()
