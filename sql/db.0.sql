-- Mise à jour vers la version 1

-- CLEAN
UPDATE ARTXRES A SET A.NBJXART = 0 WHERE A.DLVXART = 'N' AND A.NBJXART < 0;
UPDATE ARTXRES A SET A.DLVXART = 'N', A.NBJXART = 0 WHERE A.DLVXART = 'Y' AND (A.NBJXART < 1 OR A.NBJXART IS NULL);

UPDATE DARXRES S SET S.LOTXDAR = NULL WHERE char_length(S.LOTXDAR) = 0;
UPDATE DARXRES S SET S.SCCXDAR = NULL WHERE char_length(S.SCCXDAR) = 0;


UPDATE ARCXRES A SET A.QTEXARC = 1	  WHERE A.TYPXARC = 'U' and A.QTEXARC != 1;
UPDATE ARCXRES A SET A.LIBXARC = 'PALETTE' WHERE A.TYPXARC = 'P' AND ( A.LIBXARC IS NULL OR char_length(A.LIBXARC) = 0 );
UPDATE ARCXRES A SET A.LIBXARC = 'CARTON'  WHERE A.TYPXARC = 'C' AND ( A.LIBXARC IS NULL OR char_length(A.LIBXARC) = 0 );
UPDATE ARCXRES A SET A.LIBXARC = 'SACHET'  WHERE A.TYPXARC = 'S' AND ( A.LIBXARC IS NULL OR char_length(A.LIBXARC) = 0 );
UPDATE ARCXRES A SET A.LIBXARC = 'UNITE'   WHERE A.TYPXARC = 'U' AND ( A.LIBXARC IS NULL OR char_length(A.LIBXARC) = 0 );

-- Ne pas faire les deux suivantes
DELETE FROM DARXRES WHERE DARXRES IN (SELECT S.DARXRES FROM DARXRES S LEFT JOIN ADRXRES A ON S.MAGXDAR = A.MAGXADR AND S.ADRXDAR = A.CODXADR WHERE A.ADRXRES IS NULL);
DELETE FROM DARXRES WHERE DARXRES IN (SELECT D.DARXRES FROM DARXRES D LEFT JOIN ARTXRES A ON D.REFXDAR = A.REFXART AND D.DEPXDAR = A.DEPXART WHERE A.ARTXRES IS NULL);
COMMIT;

-- MAGXRES
DROP INDEX IDX_MAGXRES;
DROP TRIGGER TRIG_MAGXRES;
UPDATE RDB$RELATION_FIELDS SET RDB$NULL_FLAG = 1 WHERE RDB$FIELD_NAME = 'CODXMAG' AND RDB$RELATION_NAME = 'MAGXRES';
ALTER TABLE MAGXRES DROP MAGXRES;
ALTER TABLE MAGXRES ADD PRIMARY KEY (CODXMAG);
COMMIT;

-- ZONXRES
DROP INDEX IDX2_ZONXRES;
DROP INDEX IDX_ZONXRES;
DROP TRIGGER TRIG_ZONXRES;
UPDATE RDB$RELATION_FIELDS SET RDB$NULL_FLAG = 1 WHERE RDB$FIELD_NAME = 'CODXZON' AND RDB$RELATION_NAME = 'ZONXRES';
ALTER TABLE ZONXRES DROP ZONXRES;
ALTER TABLE ZONXRES ADD PRIMARY KEY( CODXZON );

-- DEPXRES
DROP INDEX IDX_DEPXRES;
DROP TRIGGER TRIG_DEPXRES;
UPDATE RDB$RELATION_FIELDS SET RDB$NULL_FLAG = 1 WHERE RDB$FIELD_NAME = 'CODXDEP' AND RDB$RELATION_NAME = 'DEPXRES';
ALTER TABLE DEPXRES DROP DEPXRES;
ALTER TABLE DEPXRES ADD PRIMARY KEY (CODXDEP);

-- ARTXRES
UPDATE RDB$RELATION_FIELDS SET RDB$NULL_FLAG = 1 WHERE RDB$FIELD_NAME = 'ARTXRES' AND RDB$RELATION_NAME = 'ARTXRES';
UPDATE RDB$RELATION_FIELDS SET RDB$NULL_FLAG = 1 WHERE RDB$FIELD_NAME = 'DEPXART' AND RDB$RELATION_NAME = 'ARTXRES';
UPDATE RDB$RELATION_FIELDS SET RDB$NULL_FLAG = 1 WHERE RDB$FIELD_NAME = 'REFXART' AND RDB$RELATION_NAME = 'ARTXRES';
UPDATE RDB$RELATION_FIELDS SET RDB$NULL_FLAG = 1 WHERE RDB$FIELD_NAME = 'LIBXART' AND RDB$RELATION_NAME = 'ARTXRES';

ALTER TABLE ARTXRES ADD CONSTRAINT UNIQUE_ARCXRES UNIQUE (ARTXRES);
ALTER TABLE ARTXRES ADD PRIMARY KEY (DEPXART,REFXART);

-- ADRXRES
DROP TRIGGER TRIG_ADRXRES;
DROP INDEX IDX2_ADRXRES;
DROP INDEX IDX_ADRXRES;
UPDATE RDB$RELATION_FIELDS SET RDB$NULL_FLAG = 1 WHERE RDB$FIELD_NAME = 'MAGXADR' AND RDB$RELATION_NAME = 'ADRXRES';
UPDATE RDB$RELATION_FIELDS SET RDB$NULL_FLAG = 1 WHERE RDB$FIELD_NAME = 'CODXADR' AND RDB$RELATION_NAME = 'ADRXRES';
ALTER TABLE ADRXRES DROP ADRXRES;
ALTER TABLE ADRXRES ADD PRIMARY KEY (MAGXADR,CODXADR);

-- ARCXRES
DROP TRIGGER TRIG_ARCXRES;
DROP INDEX IDX2_ARCXRES;
DROP INDEX IDX_ARCXRES;
UPDATE RDB$RELATION_FIELDS SET RDB$NULL_FLAG = 1 WHERE RDB$FIELD_NAME = 'ART_ARC' AND RDB$RELATION_NAME = 'ARCXRES';
UPDATE RDB$RELATION_FIELDS SET RDB$NULL_FLAG = 1 WHERE RDB$FIELD_NAME = 'TYPXARC' AND RDB$RELATION_NAME = 'ARCXRES';
ALTER TABLE ARCXRES DROP ARCXRES;
ALTER TABLE ARCXRES ADD PRIMARY KEY (ART_ARC, TYPXARC);

-- DARXRES
UPDATE RDB$RELATION_FIELDS SET RDB$NULL_FLAG = 1 WHERE RDB$FIELD_NAME = 'DARXRES' AND RDB$RELATION_NAME = 'DARXRES';
UPDATE RDB$RELATION_FIELDS SET RDB$NULL_FLAG = 1 WHERE RDB$FIELD_NAME = 'MAGXDAR' AND RDB$RELATION_NAME = 'DARXRES';
UPDATE RDB$RELATION_FIELDS SET RDB$NULL_FLAG = 1 WHERE RDB$FIELD_NAME = 'ADRXDAR' AND RDB$RELATION_NAME = 'DARXRES';
UPDATE RDB$RELATION_FIELDS SET RDB$NULL_FLAG = 1 WHERE RDB$FIELD_NAME = 'DEPXDAR' AND RDB$RELATION_NAME = 'DARXRES';
UPDATE RDB$RELATION_FIELDS SET RDB$NULL_FLAG = 1 WHERE RDB$FIELD_NAME = 'REFXDAR' AND RDB$RELATION_NAME = 'DARXRES';
ALTER TABLE DARXRES ALTER ADRXDAR POSITION 3;
ALTER TABLE DARXRES ADD PRIMARY KEY (DARXRES);


-- FK
ALTER TABLE ADRXRES ADD CONSTRAINT FK_ADRXRES_MAGXRES FOREIGN KEY (MAGXADR) REFERENCES MAGXRES ON DELETE CASCADE;
ALTER TABLE ADRXRES ADD CONSTRAINT FK_ADRXRES_ZONXRES FOREIGN KEY (ZONXADR) REFERENCES ZONXRES ON DELETE CASCADE;
ALTER TABLE ARCXRES ADD CONSTRAINT FK_ARCXRES_ARTXRES FOREIGN KEY (ART_ARC) REFERENCES ARTXRES (ARTXRES) ON DELETE CASCADE;
ALTER TABLE ARTXRES ADD CONSTRAINT FK_ARTXRES_DEPXART FOREIGN KEY (DEPXART) REFERENCES DEPXRES ON DELETE CASCADE ON UPDATE CASCADE;
ALTER TABLE DARXRES ADD CONSTRAINT FK_DARXRES_ADRXRES FOREIGN KEY (MAGXDAR,ADRXDAR) REFERENCES ADRXRES (MAGXADR,CODXADR) ON UPDATE CASCADE; -- Restrict ON DELETE
ALTER TABLE DARXRES ADD CONSTRAINT FK_DARXRES_ARTXRES FOREIGN KEY (DEPXDAR,REFXDAR) REFERENCES ARTXRES (DEPXART,REFXART) ON UPDATE CASCADE;  -- Restrict ON DELETE
COMMIT;

-- VIEWS
CREATE VIEW V_ARTICLE_PAR_DEPOSANT AS
SELECT
	ART.ARTXRES,
    ART.DEPXART,
    ART.REFXART,
    ART.LIBXART,
    DIM.LARXARC,
    DIM.HAUXARC,
    DIM.PROXARC,
    QTE.QTEXARC
FROM
    ARTXRES ART
    INNER JOIN ARCXRES DIM ON (ART.ARTXRES = DIM.ART_ARC AND DIM.TYPXARC = 'U')
    INNER JOIN ARCXRES QTE ON (ART.ARTXRES = QTE.ART_ARC AND QTE.TYPXARC = 'C')
;
CREATE VIEW V_ARTICLE AS
SELECT
    A.DEPXART,
    A.REFXART,
    A.LIBXART,
    A.DLVXART,
    A.LOTXART,
    A.NBJXART,
	A.ARTXRES,
    
    P.LIBXARC AS P_LIBXARC,
    P.LARXARC AS P_LARXARC,
    P.HAUXARC AS P_HAUXARC,
    P.PROXARC AS P_PROXARC,
    P.QTEXARC AS P_QTEXARC,
    P.POIXARC AS P_POIXARC,

    C.LIBXARC AS C_LIBXARC,
    C.LARXARC AS C_LARXARC,
    C.HAUXARC AS C_HAUXARC,
    C.PROXARC AS C_PROXARC,
    C.QTEXARC AS C_QTEXARC,
    C.POIXARC AS C_POIXARC,
    
    S.LIBXARC AS S_LIBXARC,
    S.LARXARC AS S_LARXARC,
    S.HAUXARC AS S_HAUXARC,
    S.PROXARC AS S_PROXARC,
    S.QTEXARC AS S_QTEXARC,
    S.POIXARC AS S_POIXARC,
    
    U.LIBXARC AS U_LIBXARC,
    U.LARXARC AS U_LARXARC,
    U.HAUXARC AS U_HAUXARC,
    U.PROXARC AS U_PROXARC,
    U.QTEXARC AS U_QTEXARC,
    U.POIXARC AS U_POIXARC
    
FROM
    ARTXRES A
    JOIN ARCXRES P ON (A.ARTXRES = P.ART_ARC AND P.TYPXARC = 'P')
    JOIN ARCXRES C ON (A.ARTXRES = C.ART_ARC AND C.TYPXARC = 'C')
    JOIN ARCXRES S ON (A.ARTXRES = S.ART_ARC AND S.TYPXARC = 'S')
    JOIN ARCXRES U ON (A.ARTXRES = U.ART_ARC AND U.TYPXARC = 'U')
;
CREATE VIEW V_STOCK_TREE AS
SELECT
    D.CODXDEP,
    D.LIBXDEP,
    M.CODXMAG,
    M.LIBXMAG,
    Z.CODXZON,
    Z.LIBXZON
FROM
    DARXRES S
    JOIN ADRXRES A ON S.ADRXDAR = A.CODXADR AND S.MAGXDAR = A.MAGXADR
    JOIN MAGXRES M ON S.MAGXDAR = M.CODXMAG
    JOIN ZONXRES Z ON A.ZONXADR = Z.CODXZON
    JOIN DEPXRES D ON S.DEPXDAR = D.CODXDEP
GROUP BY
    D.CODXDEP,
    D.LIBXDEP,
    M.CODXMAG,
    M.LIBXMAG,
    Z.CODXZON,
    Z.LIBXZON
ORDER BY
  D.LIBXDEP ASC,
  M.LIBXMAG ASC,
  Z.LIBXZON ASC	
;

CREATE VIEW V_STOCK AS
SELECT 
    ART.DEPXART AS CODXDEP,
	ADR.MAGXADR AS CODXMAG,
    ADR.ZONXADR AS CODXZON,
    ADR.CODXADR,
	ART.REFXART,
    ART.LIBXART,
    ZON.LIBXZON,
    STO.QTEXDAR,
    STO.SCCXDAR,
    STO.LOTXDAR,
    STO.DLVXDAR,
    STO.DLPXDAR,
    IIF( C.QTEXARC = 1 AND S.QTEXARC = 1, 0, FLOOR(STO.QTEXDAR / (C.QTEXARC) ) )    AS NB_C, -- Nombre de conditionnements de type C
    IIF( STO.SCCXDAR IS NULL OR ( C.QTEXARC = 1 AND S.QTEXARC = 1) , 0, FLOOR(STO.QTEXDAR / S.QTEXARC) ) AS NB_S, -- Nombre de contenants de type S
    ART.LOTXART,
    ART.DLVXART
FROM
    DARXRES STO
    JOIN ADRXRES ADR ON ( STO.MAGXDAR = ADR.MAGXADR AND STO.ADRXDAR = ADR.CODXADR )
    JOIN ZONXRES ZON ON ( ADR.ZONXADR = ZON.CODXZON )
    JOIN ARTXRES ART ON ( STO.DEPXDAR = ART.DEPXART AND STO.REFXDAR = ART.REFXART )
    JOIN ARCXRES C ON ( ART.ARTXRES = C.ART_ARC AND C.TYPXARC = 'C' )
    JOIN ARCXRES S ON ( ART.ARTXRES = S.ART_ARC AND S.TYPXARC = 'S' )
;
COMMIT;

-- VERSION
CREATE TABLE DB_INFO
(
    DB_VERSION INTEGER NOT NULL,
    DB_DATE TIMESTAMP NOT NULL
);
COMMIT;
INSERT INTO DB_INFO ( DB_VERSION, DB_DATE ) VALUES ( 1, cast( 'now' as timestamp ) );

