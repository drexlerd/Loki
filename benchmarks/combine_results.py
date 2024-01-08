import argparse
import json


def combine_json_files(files):
    combined_data = None

    for file in files:
        with open(file, 'r') as f:
            data = json.load(f)
            if combined_data is None:
                combined_data = data
            else:
                combined_data['benchmarks'].extend(data['benchmarks'])

    return combined_data


def main():
    parser = argparse.ArgumentParser(description='Combine JSON benchmark files.')
    parser.add_argument('files', nargs='+', help='JSON files to combine')
    args = parser.parse_args()

    combined_data = combine_json_files(args.files)

    print(json.dumps(combined_data, indent=4))


if __name__ == '__main__':
    main()
