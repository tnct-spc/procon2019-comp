# encording : utf-8
''' get and post json data for procon30 comp server '''

#default_token = 'procon30_example_token'

import sys
import urllib.request
import json
from urllib.error import URLError, HTTPError


args = sys.argv
json_def = {'actions': [{'agentID': 0, 'dx': 0, 'dy': 0, 'type': 'move'}]}


def postRequest(address, token, data = None):
    ''' make header & request and post it for server '''
    header = {
        'Authorization': token
    }
    if data != None:
        header['Content-Type'] = 'application/json'
        req = urllib.request.Request(address, headers=header, data=json.dumps(data).encode())
    else:
        req = urllib.request.Request(address, headers=header)

    try:
        content = urllib.request.urlopen(req)
    except HTTPError as e:
        pass
    except URLError as e:
        pass

    return json.dumps(content.read().decode('utf-8'))


def checkConnection(ip, port, token = 'procon30_example_token'):
    ''' check connection for procon30 comp server '''

    url = 'http://' + str(ip) + ':' + str(port) + '/ping'    

    return postRequest(url, token)


def getMatches(ip, port, token = 'procon30_example_token'):
    ''' get matches json data from procon30 comp server '''

    url = 'http://' + str(ip) + ':' + str(port) + '/matches'

    return postRequest(url, token)


def getMatchStatus(ip, port, token = 'procon30_example_token', matchid = 1):
    ''' get match status json data from procon30 comp server '''

    url = 'http://' + str(ip) + ':' + str(port) + '/matches/' + str(matchid)

    return postRequest(url, token)


def sendAction(ip, port, token = 'procon30_example_token', matchid = 1, json = json_def):
    ''' post action json data to procon30 comp server '''

    url = 'http://' + str(ip) + ':' + str(port) + '/matches/' + str(matchid) + '/action'

    return postRequest(url, token, json)


if __name__ == '__main__':
    print(checkConnection('localhost', 8888))
    print(getMatches('localhost', 8888))
    print(getMatchStatus('localhost', 8888, matchid=1))
    print(getMatchStatus('localhost', 8888, matchid=3))
    print(getMatchStatus('localhost', 8888, matchid=6))
    print(sendAction('localhost', 8888))

