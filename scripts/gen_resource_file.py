import os

if __name__ == '__main__':
    input_path = 'assets'
    output_path = 'resource.qrc'
    file_header = r''' <!DOCTYPE RCC><RCC version="1.0">
<qresource>'''
    file_tail = r'''</qresource>
</RCC>'''
    files = []
    for root, dirnames, filenames in os.walk(input_path):
        for filename in filenames:
            files.append(f'<file>{root}/{filename}</file>')
    content = '\n'.join([file_header, *files, file_tail])
    with open(output_path, 'w') as f:
        f.write(content)
