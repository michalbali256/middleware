import hashlib


def hash_string(string: str):
	# Hash string to integer (stable across executions, processses, nodes)
	sha256 = hashlib.sha256()
	sha256.update(string.encode('utf-8'))
	digest = sha256.hexdigest()
	return int(digest, 16)
