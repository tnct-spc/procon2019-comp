# encording : utf-8
''' get and post json data for procon30 comp server '''

#default_token = 'procon30_example_token'

import sys
import urllib.request
import json

args = sys.argv
json_def = {'actions': [{'agentID': 0, 'dx': 0, 'dy': 0, 'type': 'move'}]}

def checkConnection(ip, port, token = 'procon30_example_token'):
    '''  check connection for procon30 comp server '''

    url = 'http://' + str(ip) + ':' + str(port) + '/ping'

    header = {
        'Authorization': token
    }

    req =urllib.request.Request(url, headers=header)
    with urllib.request.urlopen(req) as content:

        return json.loads(content.read().decode('utf-8'))


def getMatches(ip, port, token = 'procon30_example_token'):
    ''' get matches json data from procon30 comp server '''

    url = 'http://' + str(ip) + ':' + str(port) + '/matches'

    headers = {
        'Authorization': token
    }

    contet = urllib.request.Request(url = url, headers=headers)

    return json.loads(content)


def getMatchStatus(ip, port, token = 'procon30_example_token', matchid = 6):
    ''' get match status json data from procon30 comp server '''

    url = 'http://' + str(ip) + ':' + str(port) + '/matches' + str(matchid)

    headers = {
        'Authorization': token
    }

    content = urllib.request.Request(url = url, headers=headers)

    return json.loads(content)


def sendAction(ip, port, token = 'procon30_example_token', matchid = 1, json = json_def):
    ''' post action json data to procon30 comp server '''

    url = 'http://' + str(ip) + ':' + str(port) + '/matches' + str(matchid) + '/action'

    headers = {
        'Content-Type': 'application/json',
        'Authorization': token
    }

    content = urllib.request.Request(url=url, headers=headers, data=json)
    f = urllib.request.urlopen(content)
    return f.read().decode('utf-8')

if __name__ == '__main__':
    if len(args) == 2:
        print(checkConnection(args[1], args[2]), args[3])
    elif len(args) == 3:
        print(checkConnection(args[1], args[2]), args[3])
    else:
        pass
