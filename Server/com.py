# encording : utf-8
''' get and post json data for procon30 comp server '''

#default_token = 'procon30_example_token'

import urllib.request
import json
from urllib.error import URLError, HTTPError

json_def = {'actions': [{'agentID': 0, 'dx': 0, 'dy': 0, 'type': 'move'}]}


def postRequest(address, token, data = None):
    ''' make header & request and post it for server '''
    header = {
        'Authorization': token
    }
    if data != None:
        header['Content-Type'] = 'application/json'
        req = urllib.request.Request(address, headers=header, data=data.encode())
    else:
        req = urllib.request.Request(address, headers=header)

    ret = {}
    try:
        content = urllib.request.urlopen(req)
    except HTTPError as e:
        ret['code'] = e.code
        ret['reason'] = e.reason
        #return json.dumps(ret)
        return e.read().decode('utf-8')
    except URLError as e:
        ret['reason'] = e.reason
        return e.reason#json.dumps(ret)
    else:
        return content.read().decode('utf-8')


def checkConnection(ip, token = 'procon30_example_token'):
    ''' check connection for procon30 comp server '''

    url = ip + '/ping'

    return postRequest(url, token)


def getMatches(ip, token = 'procon30_example_token'):
    ''' get matches json data from procon30 comp server '''

    url = ip + '/matches'

    return postRequest(url, token)


def getMatchStatus(ip, token = 'procon30_example_token', matchid = 1):
    ''' get match status json data from procon30 comp server '''

    url = ip + '/matches/' + str(matchid)

    return postRequest(url, token)


def sendAction(ip, token = 'procon30_example_token', matchid = 1, json = json_def):
    ''' post action json data to procon30 comp server '''

    url = ip + '/matches/' + str(matchid) + '/action'

    return postRequest(url, token, json)


if __name__ == '__main__':
	'''
	print('-----checkConnection()-----\n')
    print(checkConnection('localhost', 8888))
    print('\n\n-----getMatches()-----\n')
    print(getMatches('localhost', 8888))
    print('\n\n-----getMatchStatus(id=1)-----\n')
    print(getMatchStatus('localhost', 8888, matchid=1))
    print('\n\n-----getMatchStatus(id=3)-----\n')
    print(getMatchStatus('localhost', 8888, matchid=3))
    print('\n\n-----getMatchStatus(id=6)-----\n')
    print(getMatchStatus('localhost', 8888, matchid=6))
    print('\n\n-----sendAction()-----\n')
    print(sendAction('localhost', 8888))
	'''

