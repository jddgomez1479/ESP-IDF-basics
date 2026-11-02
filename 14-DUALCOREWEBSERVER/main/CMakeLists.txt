idf_component_register(SRCS "main.c"
                    INCLUDE_DIRS "."
                    REQUIRES esp_https_server
                    EMBED_TXTFILES "certificate.pem" "view.html")
