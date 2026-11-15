import os
import subprocess
import time


APP_NAME = 'built'


def setup():
    subprocess.Popen(['rm', '-rf', 'out']).wait()
    subprocess.Popen(['rm', '-f', APP_NAME]).wait()
    subprocess.Popen(['mkdir', 'out'])


def recl(spath, ignore=None):
    if ignore is None:
        ignore = []
    files = []
    if spath is None:
        iterable = os.listdir()
        spath = ''
    else:
        iterable = os.listdir(spath)
    for name in iterable:
        path = os.path.join(spath, name)
        shouldIgnore = False
        for item in ignore:
            if path.endswith(item):
                shouldIgnore = True
                break
        if shouldIgnore:
            continue
        if os.path.isfile(path):
            files.append(path)
        elif os.path.isdir(path):
            for file in recl(path):
                files.append(file)
    return files


def compile(cppfiles):
    processes = []
    for fname in cppfiles:
        builtName = f"out/{'\\'.join(fname.rstrip('.cpp').split('/'))}.o"
        # if os.path.exists(os.path.join('out', builtName)):
            # continue
        process = subprocess.Popen(['g++', '-g', '-Wall', '-c', fname, '-o', builtName])
        processes.append(process)
    # Wait for all of the processes to finish
    while True:
        atleastOneRunning = False
        for p in processes:
            if p.poll() is None:
                atleastOneRunning = True
                break
        if not atleastOneRunning:
            break


def link():
    subprocess.Popen(
            [f'g++ out/*.o -o {APP_NAME} -flto=auto'],
            shell=True
    )


def build():
    start = time.time()
    setup()
    allFiles = recl(None, ['.git', 'test'])
    cppFiles = list(filter(lambda x: x.endswith('.cpp'), allFiles))
    compile(cppFiles)
    link()
    print(f"Built in {round(time.time() - start, 5)} seconds")


build()
