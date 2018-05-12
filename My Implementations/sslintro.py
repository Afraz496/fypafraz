#!usr/bin/env python

"""
Basic use of SSL in the context of a regular cryptographic library
"""

import ssl
import socket
from pprint import pprint

HOSTNAME = "www.python.org"

context = ssl.create_default_context()
context = ssl.SSLContext(ssl.PROTOCOL_SSLv23)
context.verify_mode = ssl.CERT_REQUIRED
context.check_hostname = True
context.load_verify_locations("/etc/ssl/cert.pem")
conn = context.wrap_socket(socket.socket(socket.AF_INET), server_hostname = HOSTNAME)
conn.connect((HOSTNAME, 443))
cert = conn.getpeercert()

pprint(cert)
