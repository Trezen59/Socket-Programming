memset(buff, '\0', sizeof(buff));

                        printf("\nServer : ");

                        int bufflen;

                        // take input and store in buffer
                        int i=0;
                        while((buff[i++] = getchar()) != '\n')
                                ;
                        buff[i] = '\0';


                        // claculate buffer length and write 
                        bufflen = i-1;
                        //printf("bufflen = %d\n", bufflen);

                        *len = bufflen;

                        if((write(connfd, len, sizeof(len))) == -1)
                        {
                                printf("failed to write length\n");
                        }

                        *len = 0;

                        // if msg is more than buffer size print error msg and clear buffer
                        if(bufflen > MAX)
                        {
                                printf("msg size exceeded...!\n");
                                memset(buff, '\0', MAX);
                        }

                        // write the msg stored in buffer
                        if((write(connfd, buff, sizeof(buff))) == -1)
                        {
                                printf("failed to write \n");
                                free(len);
                                close(connfd);
                                //infi_flag = 0;
                                exit(0);
                                //printf("exiting...\n");
                        }

                        // if buffer is empty switch to read mode
                        if(strlen(buff) == 1)
                        {
                                flag = 1;
                                break;
                        }

                        // if we write bye to buffer the chat is ended
                        if(! strncmp("bye", buff, 3))
                        {
                                printf("Chat exited...!\n");
                                free(len);
                                close(connfd);
                                exit(0);
                        }
