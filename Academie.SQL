-- -----------------------------------------------------------------------------
--             Génération d'une base de données pour
--                           PostgreSQL
--                        (1/7/2025 16:43:54)
-- -----------------------------------------------------------------------------
--      Nom de la base : Academie
--      Libellé base : 
--      Projet : 
--      Auteur : nono chendjou lyrian
--      Date de dernière modification : 1/7/2025 16:42:41
-- -----------------------------------------------------------------------------

drop database Academie;
-- -----------------------------------------------------------------------------
--       CREATION DE LA BASE 
-- -----------------------------------------------------------------------------

CREATE DATABASE Academie;

-- -----------------------------------------------------------------------------
--       TABLE : FILIERE
-- -----------------------------------------------------------------------------

CREATE TABLE FILIERE
   (
    ID_FILIERE int4 NOT NULL  ,
    NOM_FILIERE char(32) NOT NULL  
,   CONSTRAINT PK_FILIERE PRIMARY KEY (ID_FILIERE)
   ) ;



-- -----------------------------------------------------------------------------
--       TABLE : ETUDIANT
-- -----------------------------------------------------------------------------

CREATE TABLE ETUDIANT
   (
    ID_ETUDIANT int4 NOT NULL  ,
    NOM char(32) NOT NULL  ,
    PRENOM char(32) NOT NULL  ,
    MATRICULE char(5) NOT NULL  ,
    TELEPHONE int4 NOT NULL  ,
    E_MAIL char(32) NOT NULL  
,   CONSTRAINT PK_ETUDIANT PRIMARY KEY (ID_ETUDIANT)
   ) ;

-- -----------------------------------------------------------------------------
--       TABLE : UTILISATEUR
-- -----------------------------------------------------------------------------

CREATE TABLE UTILISATEUR
   (
    ID_UTILISATEUR int4 NOT NULL  ,
    NOM_UTILISATEUR char(32) NOT NULL  ,
    MOT_DE_PASSE char(32) NOT NULL  
,   CONSTRAINT PK_UTILISATEUR PRIMARY KEY (ID_UTILISATEUR)
   ) ;

-- -----------------------------------------------------------------------------
--       TABLE : RENVOI_ENSEIGNANT
-- -----------------------------------------------------------------------------

CREATE TABLE RENVOI_ENSEIGNANT
   (
    ID_RENVOI int4 NOT NULL  ,
    ID_ENSEIGNANT int4 NOT NULL  ,
    MOTIF char(255) NOT NULL  
,   CONSTRAINT PK_RENVOI_ENSEIGNANT PRIMARY KEY (ID_RENVOI)
   ) ;

-- -----------------------------------------------------------------------------
--       INDEX DE LA TABLE RENVOI_ENSEIGNANT
-- -----------------------------------------------------------------------------

CREATE  INDEX I_FK_RENVOI_ENSEIGNANT_ENSEIGN
     ON RENVOI_ENSEIGNANT (ID_ENSEIGNANT)
    ;

-- -----------------------------------------------------------------------------
--       TABLE : ENSEIGNANTS
-- -----------------------------------------------------------------------------

CREATE TABLE ENSEIGNANTS
   (
    ID_ENSEIGNANT int4 NOT NULL  ,
    NOM char(32) NOT NULL  ,
    PRENOM char(32) NOT NULL  ,
    MATRICULE int4 NOT NULL  ,
    ADRESSE char(32) NOT NULL  ,
    TELEPHONE int8 NOT NULL  
,   CONSTRAINT PK_ENSEIGNANTS PRIMARY KEY (ID_ENSEIGNANT)
   ) ;

-- -----------------------------------------------------------------------------
--       TABLE : RENVOI_ETUDIANT
-- -----------------------------------------------------------------------------

CREATE TABLE RENVOI_ETUDIANT
   (
    ID_RENVOI int2 NOT NULL  ,
    ID_ETUDIANT int4 NOT NULL  ,
    MOTIF char(255) NOT NULL  
,   CONSTRAINT PK_RENVOI_ETUDIANT PRIMARY KEY (ID_RENVOI)
   ) ;

-- -----------------------------------------------------------------------------
--       INDEX DE LA TABLE RENVOI_ETUDIANT
-- -----------------------------------------------------------------------------

CREATE  INDEX I_FK_RENVOI_ETUDIANT_ETUDIANT
     ON RENVOI_ETUDIANT (ID_ETUDIANT)
    ;

-- -----------------------------------------------------------------------------
--       TABLE : CLASSE
-- -----------------------------------------------------------------------------

CREATE TABLE CLASSE
   (
    ID_CLASSE int2 NOT NULL  ,
    NOM_CLASSE char(32) NOT NULL,   
 ID_FILIERE int4 NOT NULL  ,
CONSTRAINT PK_CLASSE PRIMARY KEY (ID_CLASSE)
   ) ;
-- -----------------------------------------------------------------------------
--       INDEX DE LA TABLE CLASSE
-- -----------------------------------------------------------------------------

CREATE  INDEX I_FK_CLASSE_FILIERE
     ON CLASSE (ID_FILIERE)
    ;

-- -----------------------------------------------------------------------------
--       TABLE : MATIERES
-- -----------------------------------------------------------------------------

CREATE TABLE MATIERES
   (
    ID_MATIERE int4 NOT NULL  ,
    NOM_MATIERE char(32) NOT NULL 
 ID_FILIERE int4 NOT NULL, 
   CONSTRAINT PK_MATIERES PRIMARY KEY (ID_MATIERE)
   ) ;
-- -----------------------------------------------------------------------------
--       INDEX DE LA TABLE MATIERES
-- -----------------------------------------------------------------------------

CREATE  INDEX I_FK_MATIERES_FILIERE
     ON MATIERES (ID_FILIERE)
    ;
-- -----------------------------------------------------------------------------
--       TABLE : PROMOUVOIR_ENSEIGNANTS
-- -----------------------------------------------------------------------------

CREATE TABLE PROMOUVOIR_ENSEIGNANTS
   (
    ID_PROMOTION int2 NOT NULL  ,
    ID_ENSEIGNANT int4 NOT NULL  
,   CONSTRAINT PK_PROMOUVOIR_ENSEIGNANTS PRIMARY KEY (ID_PROMOTION)
   ) ;

-- -----------------------------------------------------------------------------
--       INDEX DE LA TABLE PROMOUVOIR_ENSEIGNANTS
-- -----------------------------------------------------------------------------

CREATE  INDEX I_FK_PROMOUVOIR_ENSEIGNANTS_EN
     ON PROMOUVOIR_ENSEIGNANTS (ID_ENSEIGNANT)
    ;

-- -----------------------------------------------------------------------------
--       TABLE : EMPLOIE_DE_TEMPS
-- -----------------------------------------------------------------------------

CREATE TABLE EMPLOIE_DE_TEMPS
   (
    ID_EMPLOI int4 NOT NULL  ,
    ID_ENSEIGNANT int4 NOT NULL  ,
    ID_MATIERE int4 NOT NULL  ,
    ID_CLASSE int2 NOT NULL  
,   CONSTRAINT PK_EMPLOIE_DE_TEMPS PRIMARY KEY (ID_EMPLOI)
   ) ;

-- -----------------------------------------------------------------------------
--       INDEX DE LA TABLE EMPLOIE_DE_TEMPS
-- -----------------------------------------------------------------------------

CREATE  INDEX I_FK_EMPLOIE_DE_TEMPS_ENSEIGNA
     ON EMPLOIE_DE_TEMPS (ID_ENSEIGNANT)
    ;

CREATE  INDEX I_FK_EMPLOIE_DE_TEMPS_MATIERES
     ON EMPLOIE_DE_TEMPS (ID_MATIERE)
    ;

CREATE  INDEX I_FK_EMPLOIE_DE_TEMPS_CLASSE
     ON EMPLOIE_DE_TEMPS (ID_CLASSE)
    ;


-- -----------------------------------------------------------------------------
--       CREATION DES REFERENCES DE TABLE
-- -----------------------------------------------------------------------------


ALTER TABLE MATIERES ADD 
     CONSTRAINT FK MATIERES_FILIERE
          FOREIGN KEY (ID_FILIERE)
               REFERENCES FILIERE (ID_FILIERE);

ALTER TABLE RENVOI_ENSEIGNANT ADD 
     CONSTRAINT FK_RENVOI_ENSEIGNANT_ENSEIGNANTS
          FOREIGN KEY (ID_ENSEIGNANT)
               REFERENCES ENSEIGNANTS (ID_ENSEIGNANT);

ALTER TABLE RENVOI_ETUDIANT ADD 
     CONSTRAINT FK_RENVOI_ETUDIANT_ETUDIANT
          FOREIGN KEY (ID_ETUDIANT)
               REFERENCES ETUDIANT (ID_ETUDIANT);

ALTER TABLE PROMOUVOIR_ENSEIGNANTS ADD 
     CONSTRAINT FK_PROMOUVOIR_ENSEIGNANTS_ENSEIG
          FOREIGN KEY (ID_ENSEIGNANT)
               REFERENCES ENSEIGNANTS (ID_ENSEIGNANT);

ALTER TABLE EMPLOIE_DE_TEMPS ADD 
     CONSTRAINT FK_EMPLOIE_DE_TEMPS_ENSEIGNANTS
          FOREIGN KEY (ID_ENSEIGNANT)
               REFERENCES ENSEIGNANTS (ID_ENSEIGNANT);

ALTER TABLE EMPLOIE_DE_TEMPS ADD 
     CONSTRAINT FK_EMPLOIE_DE_TEMPS_MATIERES
          FOREIGN KEY (ID_MATIERE)
               REFERENCES MATIERES (ID_MATIERE);

ALTER TABLE EMPLOIE_DE_TEMPS ADD 
     CONSTRAINT FK_EMPLOIE_DE_TEMPS_CLASSE
          FOREIGN KEY (ID_CLASSE)
               REFERENCES CLASSE (ID_CLASSE);

ALTER TABLE CLASSE ADD 
     CONSTRAINT FK CLASSE_FILIERE
          FOREIGN KEY (ID_FILIERE)
               REFERENCES FILIERE (ID_FILIERE);

-- -----------------------------------------------------------------------------
--                FIN DE GENERATION
-- -----------------------------------------------------------------------------