import json
import os
import hashlib
import random

chain_dir = os.curdir + '/chain/'
usernames = ['Satoshi', 'Oleg', 'Olga', 'Aleksandr', 'Sasha', 'Dmitry', 'Egor', 'Andrii', 'Maxim', 'Vasja', 'Yozya',
             'Maria', 'Irina', 'Anastasia', 'Helen', 'Sergii', 'Anton']
amount_max = 1000000
max_transaction = 10000


def get_hash(filename):
    file = open(chain_dir + filename, 'rb').read()
    return hashlib.sha256(file).hexdigest()


def check_corruptions():
    files = os.listdir(chain_dir)
    files = sorted([int(i) for i in files])

    for file in files[1:]:
        f = open(chain_dir + str(file))
        h = json.load(f)['prev_hash']

        prev_file = str(file - 1)
        actual_hash = get_hash(prev_file)

        if h == actual_hash:
            res = 'Ok'
        else:
            res = 'Corrupted'

        print('Block {} is {}'.format(prev_file, res))


def write_block(transactions, prev_hash=''):
    files = os.listdir(chain_dir)
    files = sorted([int(i) for i in files])

    last_file = files[-1]

    filename = str(last_file + 1)

    prev_hash = get_hash(str(last_file))

    data = {
        'transactions': transactions,
        'nonce': 0,
        'prev_hash': prev_hash
    }

    with open(chain_dir + filename, 'w') as file:
        json.dump(data, file, indent=4)

    while not str(get_hash(filename)).startswith('0'):
        data['nonce'] += random.randint(10, 1000)
        with open(chain_dir + filename, 'w') as file:
            json.dump(data, file, indent=4)


def generate_transactions(size):
    transactions = []
    for i in range(size):
        name = usernames[random.randint(1, len(usernames) - 1)]
        to_whom = name
        while to_whom == name:
            to_whom = usernames[random.randint(1, len(usernames) - 1)]
        wallet = check_user_wallet(name)
        amount = random.randint(1, int(wallet/2))
        transaction = {
            'name': name,
            'to_whom': to_whom,
            'amount': amount
        }
        transactions.append(transaction)

    return transactions


def generate_blocks(size):
    for i in range(0, size):
        num_of_transactions = random.randint(10, max_transaction)
        transactions = generate_transactions(num_of_transactions)
        write_block(transactions)


def check_user_wallet(username):
    files = os.listdir(chain_dir)
    files = sorted([int(i) for i in files])
    wallet = 0
    for file in files[1:]:
        f = open(chain_dir + str(file))
        bl = json.load(f)
        for transaction in bl['transactions']:
            if transaction['name'] == username:
                wallet -= transaction['amount']
            if transaction['to_whom'] == username:
                wallet += transaction['amount']
    return wallet

def satoshi_help():
    transactions = []
    for i in range(len(usernames)-1):
        name = usernames[0]
        to_whom = usernames[i+1]
        amount = random.randint(10000, amount_max)
        transaction = {
            'name': name,
            'to_whom': to_whom,
            'amount': amount
        }
        transactions.append(transaction)

    write_block(transactions)

def get_money_in_block(filename):
    value = 0
    with open(chain_dir + str(filename)) as file:
        data = json.load(file)['transactions']
    for transaction in data:
        value += transaction['amount']
    return value

def main():
    print(get_money_in_block(2))


if __name__ == '__main__':
    main()
