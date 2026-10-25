-- FUNCTION: public.fn_empresas_bu0()

-- DROP FUNCTION IF EXISTS public.fn_empresas_bu0();

CREATE OR REPLACE FUNCTION public.fn_empresas_bu0()
    RETURNS trigger
    LANGUAGE 'plpgsql'
    COST 100
    VOLATILE NOT LEAKPROOF
AS $BODY$
	BEGIN
		NEW.atualizado_em := CURRENT_TIMESTAMP;
		RETURN NEW;
	END;
$BODY$;

-- Trigger: empresas_bu0

-- DROP TRIGGER IF EXISTS empresas_bu0 ON public.empresas;

CREATE OR REPLACE TRIGGER empresas_bu0
    BEFORE UPDATE
    ON public.empresas
    FOR EACH ROW
    EXECUTE FUNCTION public.fn_empresas_bu0();