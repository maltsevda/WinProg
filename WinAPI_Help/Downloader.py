#!/usr/bin/env python
"""Microsoft's Old API Help File Reborn"""

from urllib.request import urlopen

#
# download(url, name)
#

def download(url, name):

    """Download binary file from url to disk"""

    max_chunk = 1024 * 16

    with urlopen(url) as response:
        with open(name, 'wb') as file:
            while True:
                chunk = response.read(max_chunk)
                if not chunk:
                    break
                file.write(chunk)

#
# main() - entry point
#

def main():

    """Entry point"""

    print('Downloading, please wait...')
    download('http://laurencejackson.com/win32/Win32.chm', 'Win32.chm')
    print('Download complete.')

#
# start script
#

if __name__ == '__main__':
    main()
