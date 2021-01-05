CREATE SCHEMA `IOTcasa`;

CREATE TABLE `IOTcasa`.tipo_disp ( 
	tipo_disp_id         bigint  NOT NULL  AUTO_INCREMENT  PRIMARY KEY,
	descripcion          varchar(200)  NOT NULL    
 );

ALTER TABLE `IOTcasa`.tipo_disp MODIFY descripcion varchar(200)  NOT NULL   COMMENT 'Nombre y caracteristicas del dispositivo';

CREATE TABLE `IOTcasa`.tipo_med ( 
	tipo_med_id          bigint  NOT NULL  AUTO_INCREMENT  PRIMARY KEY,
	descripcion          varchar(100)  NOT NULL    
 );

ALTER TABLE `IOTcasa`.tipo_med MODIFY descripcion varchar(100)  NOT NULL   COMMENT 'Temperatura, Humedad, etc';

CREATE TABLE `IOTcasa`.usuario ( 
	usuario_id           bigint  NOT NULL  AUTO_INCREMENT  PRIMARY KEY,
	nombre               varchar(40)  NOT NULL    ,
	cargo                varchar(40)      
 );

CREATE TABLE `IOTcasa`.dispositivos ( 
	dispositivo_id       bigint  NOT NULL  AUTO_INCREMENT  PRIMARY KEY,
	ubicacion            varchar(200)      ,
	tipo_disp_id         bigint  NOT NULL    ,
	CONSTRAINT fk_dispositivos_tipo_disp FOREIGN KEY ( tipo_disp_id ) REFERENCES `IOTcasa`.tipo_disp( tipo_disp_id ) ON DELETE RESTRICT ON UPDATE RESTRICT
 );

CREATE INDEX fk_dispositivos_tipo_disp ON `IOTcasa`.dispositivos ( tipo_disp_id );

ALTER TABLE `IOTcasa`.dispositivos MODIFY ubicacion varchar(200)     COMMENT 'Ubicacion del dispositivo';

CREATE TABLE `IOTcasa`.estadodispositivo ( 
	estado_id            bigint  NOT NULL  AUTO_INCREMENT  PRIMARY KEY,
	estado               int  NOT NULL    ,
	timetag              bigint  NOT NULL    ,
	dispositivo_id       bigint  NOT NULL    ,
	CONSTRAINT fk_estadodispositivo FOREIGN KEY ( dispositivo_id ) REFERENCES `IOTcasa`.dispositivos( dispositivo_id ) ON DELETE RESTRICT ON UPDATE RESTRICT
 );

CREATE INDEX fk_estadodispositivo ON `IOTcasa`.estadodispositivo ( dispositivo_id );

ALTER TABLE `IOTcasa`.estadodispositivo MODIFY dispositivo_id bigint  NOT NULL   COMMENT 'id del dispositivo que se dio de alta o baja';

CREATE TABLE `IOTcasa`.medicion ( 
	medicion_id          bigint  NOT NULL  AUTO_INCREMENT  PRIMARY KEY,
	valor                float(12,0)  NOT NULL    ,
	tipo_med_id          bigint  NOT NULL    ,
	timetag              bigint  NOT NULL    ,
	dispositivo_id       bigint  NOT NULL    ,
	CONSTRAINT fk_medicion_dispositivos FOREIGN KEY ( dispositivo_id ) REFERENCES `IOTcasa`.dispositivos( dispositivo_id ) ON DELETE RESTRICT ON UPDATE RESTRICT,
	CONSTRAINT fk_tipo_medicion FOREIGN KEY ( tipo_med_id ) REFERENCES `IOTcasa`.tipo_med( tipo_med_id ) ON DELETE RESTRICT ON UPDATE RESTRICT
 );

CREATE INDEX fk_medicion_dispositivos ON `IOTcasa`.medicion ( dispositivo_id );

CREATE INDEX fk_tipo_medicion ON `IOTcasa`.medicion ( tipo_med_id );

CREATE TABLE `IOTcasa`.accion ( 
	accion_id            bigint  NOT NULL  AUTO_INCREMENT  PRIMARY KEY,
	dispositivo_id       bigint  NOT NULL    ,
	usuario_id           bigint  NOT NULL    ,
	salida               int      ,
	estado               float(12,0)      ,
	timetag              bigint  NOT NULL    ,
	CONSTRAINT fk_accion_dispositivos FOREIGN KEY ( dispositivo_id ) REFERENCES `IOTcasa`.dispositivos( dispositivo_id ) ON DELETE RESTRICT ON UPDATE RESTRICT,
	CONSTRAINT fk_accion_usuario FOREIGN KEY ( usuario_id ) REFERENCES `IOTcasa`.usuario( usuario_id ) ON DELETE RESTRICT ON UPDATE RESTRICT
 );

CREATE INDEX fk_accion_dispositivos ON `IOTcasa`.accion ( dispositivo_id );

CREATE INDEX fk_accion_usuario ON `IOTcasa`.accion ( usuario_id );

ALTER TABLE `IOTcasa`.accion MODIFY dispositivo_id bigint  NOT NULL   COMMENT 'En que dispositivo se realizo la accion';

ALTER TABLE `IOTcasa`.accion MODIFY salida int     COMMENT 'Que salida se mando a operar';

ALTER TABLE `IOTcasa`.accion MODIFY estado float(12,0)     COMMENT 'Que estado se mando a escribir';
