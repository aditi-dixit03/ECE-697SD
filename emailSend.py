#!/usr/bin/env python

import os
import sys
import smtplib
from email import encoders
from email.mime.base import MIMEBase
from email.mime.text import MIMEText
from email.mime.multipart import MIMEMultipart

COMMASPACE = ', '

def main():
    sender = 'adixit1234567@gmail.com'
    gmail_password = 'ECE697networkembed'
    #recipients = ['patle.kalyani@gmail.com']
    recipients = sys.argv[1]

    # Create the enclosing (outer) message
    outer = MIMEMultipart()
    outer['Subject'] = 'Test'
    #outer['To'] = COMMASPACE.join(recipients)
    outer['To'] = recipients
    outer['From'] = sender
    outer.preamble = 'You will not see this in a MIME-aware mail reader.\n'

    # List of attachments
    #attachments = sys.argv[1]
    file = sys.argv[2]

    # Add the attachments to the message
    #for file in attachments:
    try:
        with open(file, 'rb') as fp:
            msg = MIMEBase('application', "octet-stream")
            msg.set_payload(fp.read())
        encoders.encode_base64(msg)
        msg.add_header('Content-Disposition', 'attachment', filename=os.path.basename(file))
        text_msg = MIMEText("Output File", "plain")
        outer.attach(msg)
        outer.attach(text_msg)
    except:
        print("Unable to open one of the attachments. Error: ", sys.exc_info()[0])
        raise
    composed = outer.as_string()

    try:
        with smtplib.SMTP('smtp.gmail.com', 587) as s:
            s.ehlo()
            s.starttls()
            s.ehlo()
            s.login(sender, gmail_password)
            s.sendmail(sender, recipients, composed)
            s.close()
        print("Email sent to: '"+recipients+"' successfully!")
    except:
        print("Unable to send the email. Error: ", sys.exc_info()[0])
        raise

if __name__ == '__main__':
        main()
        

